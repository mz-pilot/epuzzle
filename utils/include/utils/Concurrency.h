#pragma once
#include <algorithm>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

#include "Diagnostics.h"

namespace utils
{

    // A simple analogue of a std::latch with a wait_for() method (std::latch hasn't it).
    class TimedLatch
    {
    public:
        explicit TimedLatch(size_t expected)
            : m_counter(expected)
        {
            ENSURE(expected > 0, "");
        }

        void countDown()
        {
            bool lastReached = false;
            {
                std::lock_guard lock{ m_mx };
                if (m_counter > 0)
                    lastReached = (--m_counter == 0);
            }
            if (lastReached)
                m_cv.notify_all();
        }

        // return true, if all threads done, false - if timeout
        template <class TRep, class TPeriod>
        bool waitFor(const std::chrono::duration<TRep, TPeriod>& relTime)
        {
            std::unique_lock lock{ m_mx };
            return m_cv.wait_for(lock, relTime, [this] { return m_counter == 0; });
        }

    private:
        std::mutex m_mx;
        std::condition_variable m_cv;
        size_t m_counter = 0;
    };


    template <typename TResult> // Like "fan-out" pattern. Note: TResult=void not supported. If some thread fail - all threads stopped.
    class ParallelExecutor
    {
    public:
        using ThreadFun = std::function<TResult(std::stop_token)>;
        ParallelExecutor(size_t count, ThreadFun fun)
            : m_monitor(count)
        {
            for (size_t i = 0; i < count; ++i)
            {
                auto monitoredFun = [this, fun](std::stop_token st) // copy for each thread
                    {
                        try
                        {
                            const auto threadResult = fun(st);
                            m_monitor.countDown(); // thread done
                            return threadResult;
                        }
                        catch (...)
                        {
                            m_monitor.countDown(); // thread done
                            request_stop();
                            throw;
                        }
                    };
                std::packaged_task<TResult(std::stop_token)> ptask{ std::move(monitoredFun) };
                auto future = ptask.get_future();
                m_workers.emplace_back(std::move(future), std::jthread(std::move(ptask)));
            }
        }

        // return true, if all tasks done, false - if timeout
        template <class TRep, class TPeriod>
        bool waitFor(const std::chrono::duration<TRep, TPeriod>& relTime)
        {
            return m_monitor.waitFor(relTime);
        }

        void request_stop()
        {
            std::ranges::for_each(m_workers, [](auto& worker) { worker.thread.request_stop(); });
        }

        std::vector<TResult> get() // throw ExceptionMessageCollector
        {
            std::vector<TResult> results;
            results.reserve(m_workers.size());
            ExceptionMessageCollector errMsgCollector("ParallelExecutor detected some exceptions!");
            for (auto& worker : m_workers)
            {
                try
                {
                    results.push_back(worker.future.get());
                }
                catch (const std::exception& exc)
                {
                    errMsgCollector.add(exc.what());
                }
                catch (...)
                {
                    errMsgCollector.add("catch(...)"); // Let's leave it that way for now. Improve with exception_ptr later.
                }
            }
            if (errMsgCollector.count() > 0)
                throw errMsgCollector;

            return results;
        }

    private:
        struct ThreadWorker
        {
            std::future<TResult> future;
            std::jthread thread;
        };

        TimedLatch m_monitor;
        std::vector<ThreadWorker> m_workers;
    };

}
