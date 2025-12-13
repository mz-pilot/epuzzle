#pragma once
#include <algorithm>
#include <atomic>
#include <chrono>
#include <concepts>
#include <cstdint>

#include "Hardware.h"
#include "Math.h"

namespace utils
{

    // For hot cycles, can be shared between threads. 
    class AtomicProgressTracker
    {
    public:
        explicit AtomicProgressTracker(std::uint64_t countInterval) noexcept
            : m_totalProgress{ 0 }
            , m_countInterval(countInterval)
        {
        }

        // Don't share between threads!
        class LocalTracker
        {
            friend AtomicProgressTracker;
            LocalTracker(const LocalTracker&) = delete;
            LocalTracker(LocalTracker&&) = delete;
            LocalTracker& operator= (const LocalTracker&) = delete;
            LocalTracker& operator= (LocalTracker&&) = delete;

        public:
            void update() noexcept
            {
                if (++m_delta >= m_countInterval) [[unlikely]]
                    store();
            }

            void finish() noexcept
            {
                if (m_delta)
                    store();
            }

            ~LocalTracker() noexcept
            {
                finish();
            }

        private:
            LocalTracker(AtomicProgressTracker& owner, std::uint64_t countInterval) noexcept
                : m_countInterval(countInterval)
                , m_owner(owner)
            {
            }

            void store() noexcept
            {
                m_owner.m_totalProgress.fetch_add(m_delta, std::memory_order_relaxed);
                m_delta = 0;
            }

        private:
            const std::uint64_t m_countInterval;
            AtomicProgressTracker& m_owner;
            std::uint64_t m_delta = 0;
        };

        LocalTracker getLocalTracker() noexcept
        {
            return LocalTracker{ *this, m_countInterval }; // guaranteed copy elision since C++17
        }

        std::uint64_t load() const noexcept
        {
            return m_totalProgress.load(std::memory_order_relaxed);
        }

    private:
        alignas(hwL1CacheLineSize()) std::atomic<std::uint64_t> m_totalProgress; // avoid false sharing.
        const std::uint64_t m_countInterval; // same cache line but very rare access
    };
    static_assert(sizeof(AtomicProgressTracker) == hwL1CacheLineSize());
    static_assert(alignof(AtomicProgressTracker) == hwL1CacheLineSize());


    template<typename T>
    concept TotalProgressCallback = std::is_invocable_r_v<bool, T, std::uint64_t, std::uint64_t>;
    // For hot cycles, single threaded automatical object. Callback with 2 parameters: total, current. Callback return false if user canceled.
    template <TotalProgressCallback TCallback>
    class ProgressTracker
    {
    public:
        ProgressTracker(std::uint64_t total, std::chrono::milliseconds minTimeInterval, std::uint64_t minCountInterval, TCallback onProgress)
            : m_onProgress(std::move(onProgress))
            , m_total(std::max(total, 1_u64))
            , m_minTimeInterval(minTimeInterval)
            , m_minCountInterval(std::min(minCountInterval, m_total))
        {
            call(0);
        }

        // Return false if user canceled (callback returned false)
        bool update()
        {
            if (++m_processed == m_total) [[unlikely]]
            {
                finish();
                return true;
            }

            if (++m_countSinceCall >= m_minCountInterval) [[unlikely]]
            {
                m_countSinceCall = 0;
                const auto now = std::chrono::steady_clock::now();
                if (now - m_lastCallTimePoint >= m_minTimeInterval)
                    return call(m_processed, now);
            }

            return true;
        }

        void finish()
        {
            call(m_total);
        }

    private:
        using steady_tp = std::chrono::steady_clock::time_point;

        bool call(std::uint64_t processed, steady_tp timePoint = std::chrono::steady_clock::now())
        {
            m_countSinceCall = 0;
            m_lastCallTimePoint = timePoint;
            return m_onProgress(m_total, processed);
        }

    private:
        TCallback m_onProgress; // non-const: it can be mutable lamdba
        const std::uint64_t m_total;
        const std::chrono::milliseconds m_minTimeInterval;
        const std::uint64_t m_minCountInterval;
        steady_tp m_lastCallTimePoint;
        std::uint64_t m_countSinceCall = 0;
        std::uint64_t m_processed = 0;
    };

}
