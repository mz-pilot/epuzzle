#pragma once
#include <cstdint>
#include <iterator>
#include <ostream>
#include <string_view>
#include <string>
#include <utility>
#include <vector>

#include "Diagnostics.h"

namespace utils
{

    // Expect UTF8
    class StringTablePrinter
    {
    public:
        explicit StringTablePrinter(std::string_view delimiter, std::string_view firstColumnPrepend = "")
            : m_delim(delimiter)
            , m_rowPrepend(firstColumnPrepend)
        {
        }

        void reserve(size_t rows)
        {
            m_table.reserve(rows);
        }

        void addRow(std::vector<std::string> row)
        {
            if (m_columnWidths.empty())
                m_columnWidths.resize(row.size());

            ENSURE(m_columnWidths.size() == row.size(), "All rows must be the same size!");

            for (size_t i = 0; i < row.size(); ++i)
                m_columnWidths[i] = std::max(m_columnWidths[i], utf8Length(row[i]));

            m_table.push_back(std::move(row));
        }

        void addRow(std::string_view firstColumn, std::vector<std::string> row)
        {
            std::vector<std::string> fullRow;
            fullRow.reserve(1 + row.size());
            fullRow.emplace_back(firstColumn);
            std::move(row.begin(), row.end(), std::back_inserter(fullRow));
            addRow(std::move(fullRow));
        }

        friend std::ostream& operator<< (std::ostream& os, const StringTablePrinter& printer)
        {
            for (const auto& row : printer.m_table)
            {
                os << printer.m_rowPrepend;
                for (size_t i = 0; i < row.size(); ++i)
                {
                    if (i > 0)
                        os << printer.m_delim;

                    os << row[i];

                    const auto itemLength = utf8Length(row[i]);
                    if (itemLength < printer.m_columnWidths[i])
                        os << std::string(printer.m_columnWidths[i] - itemLength, ' ');
                }

                os << "\n";
            }
            return os;
        }

    private:
        // Warn! approximate UTF-8 aware length calculation (counts code points, not bytes, not graphemes).
        static size_t utf8Length(const std::string& s)
        {
            size_t count = 0;
            for (std::uint8_t byte : s)
                if ((byte & 0xC0) != 0x80) // count only leading bytes (not continuation bytes 10xxxxxx)
                    ++count;
            return count;
        };

    private:
        std::vector<std::vector<std::string>> m_table;
        std::vector<size_t> m_columnWidths;

        const std::string m_delim;
        const std::string m_rowPrepend;
    };

}
