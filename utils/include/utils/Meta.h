#pragma once
#include <algorithm>
#include <concepts>
#include <iterator>
#include <utility>
#include <vector>

#include "Diagnostics.h"

namespace utils
{

    // Something like BOOST_STRONG_TYPEDEF for using in IndexedVector (see below)
    template <typename Tag, typename T = size_t>
    class TypedIndex
    {
    public:
        explicit constexpr TypedIndex(T v = T{}) : m_value(v) {}
        template <std::random_access_iterator Iterator>
        static TypedIndex fromDistance(Iterator current, Iterator first)
        {
            const auto dist = current - first;
            ENSURE(dist >= 0, "negative distance!");
            return TypedIndex{ static_cast<T>(dist) };
        }

        constexpr T value() const noexcept { return m_value; }
        constexpr auto operator <=>(const TypedIndex&) const = default;
        constexpr TypedIndex& operator++() noexcept { ++m_value; return *this; }
        constexpr TypedIndex& operator--() noexcept { --m_value; return *this; }
        constexpr TypedIndex operator++(int) noexcept { TypedIndex temp = *this; ++m_value; return temp; }
        constexpr TypedIndex operator--(int) noexcept { TypedIndex temp = *this; --m_value; return temp; }

    private:
        T m_value;
    };
    static_assert(std::regular<TypedIndex<struct static_test_tag>>);
    static_assert(std::totally_ordered<TypedIndex<struct static_test_tag>>);


    // Use TypedIndex (see above) as TIndex
    template <typename TIndex, typename TValue>
    class IndexedVector
    {
    public:
        using UnderType = std::vector<TValue>;
        using value_type = UnderType::value_type;
        using size_type = UnderType::size_type;
        using iterator = UnderType::iterator;
        using const_iterator = UnderType::const_iterator;

        IndexedVector() = default;
        explicit IndexedVector(const size_type count) : m_data(count) {}
        IndexedVector(std::vector<TValue>&& data) : m_data(std::move(data)) {}

        constexpr void reserve(size_type capacity) { return m_data.reserve(capacity); }
        constexpr size_type capacity() const noexcept { return m_data.capacity(); }
        constexpr size_type size() const noexcept { return m_data.size(); }
        constexpr TValue& operator[](TIndex index) { return m_data[index.value()]; }
        constexpr const TValue& operator[](TIndex index) const { return m_data[index.value()]; }
        constexpr TValue& front() { return m_data.front(); }
        constexpr const TValue& front() const { return m_data.front(); }
        constexpr TValue& back() { return m_data.back(); }
        constexpr const TValue& back() const { return m_data.back(); }

        constexpr iterator begin() noexcept { return m_data.begin(); }
        constexpr const_iterator begin() const noexcept { return m_data.begin(); }
        constexpr const_iterator cbegin() const noexcept { return m_data.cbegin(); }
        constexpr iterator end() noexcept { return m_data.end(); }
        constexpr const_iterator end() const noexcept { return m_data.end(); }
        constexpr const_iterator cend() const noexcept { return m_data.cend(); }

        constexpr void push_back(const TValue& value) { m_data.push_back(value); }
        constexpr void push_back(TValue&& value) { m_data.push_back(std::move(value)); }
        template <class... TArgs>
        constexpr void emplace_back(TArgs&&... args) { m_data.emplace_back(std::forward<TArgs>(args)...); }

    private:
        UnderType m_data;
    };


    template <typename T>
    std::vector<T> join(std::vector<std::vector<T>>&& parts)
    {
        size_t targetSize = 0;
        std::ranges::for_each(parts, [&targetSize](const std::vector<T>& part) { targetSize += part.size(); });

        std::vector<T> joined;
        joined.reserve(targetSize);
        for (auto& part : parts)
            std::move(part.begin(), part.end(), std::back_inserter(joined));

        parts.clear();
        return joined;
    }


    // Helper type for std:visit()
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
    // enough, we have c++20

}
