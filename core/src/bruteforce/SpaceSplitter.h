#pragma once

namespace epuzzle::details::bruteforce
{
    // Splits a set into fragments of a specified size in a multi-threaded environment
    class SpaceSplitter
    {
    public:
        explicit SpaceSplitter(std::uint64_t totalItems, std::uint64_t chunkSize = 10'000'000) // chunkSize could be specified in the config-file
            : m_totalItems(totalItems)
            , m_chunkSize(chunkSize)
        {
        }

        struct Chunk
        {
            std::uint64_t offset = 0;
            std::uint64_t count = 0;
        };

        std::optional<Chunk> nextChunk()
        {
            const std::lock_guard<std::mutex>lock(m_distributedGuard);
            if (m_distributed >= m_totalItems)
                return {};

            const Chunk chunk{ .offset = m_distributed, .count = std::min(m_chunkSize, m_totalItems - m_distributed) };
            m_distributed += chunk.count;
            return chunk;
        }

    private:
        const std::uint64_t m_totalItems;
        const std::uint64_t m_chunkSize;

        std::uint64_t m_distributed = 0;
        std::mutex m_distributedGuard;
    };
}
