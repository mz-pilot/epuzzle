#pragma once
#include <chrono>
#include <format>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

// If the expression is false, throw specException constructed with the message. 
// Usage example: ENSURE_SPEC(i < 50, my_custom_exception, "i = " << i << ". Other text/params.");
#define ENSURE_SPEC(expression, specException, message) \
	do { \
		if (!(expression)) \
		{ \
			std::ostringstream oss; \
			const auto loc = std::source_location::current(); \
			oss << "ENSURE( " #expression " );\nDetails: " << message \
                << "\nFile: " << loc.file_name() << ", line: " << loc.line() << "\nFunction: " << loc.function_name(); \
			throw specException(oss.str()); \
		} \
	} while (false)

// If the expression is false, throw std::runtime_error constructed with the message. Usage example: ENSURE(i < 50, "i = " << i << ". Other text or params.");
#define ENSURE(expression, message) \
        ENSURE_SPEC(expression, std::runtime_error, message)

namespace utils
{

    class Stopwatch
    {
    public:
        Stopwatch() : m_time0(std::chrono::steady_clock::now()) {}
        std::chrono::duration<double> elapsed() const { return std::chrono::steady_clock::now() - m_time0; }

    private:
        const std::chrono::steady_clock::time_point m_time0;
    };


    // Don't share between threads! Cheap alternative without exception_ptr aggregation.
    class ExceptionMessageCollector final : public std::runtime_error
    {
    public:
        explicit ExceptionMessageCollector(std::string_view headerDescription) : runtime_error(""), m_msg(headerDescription) {}
        void add(std::string_view what) { m_msg += std::format("\nStored exception description #{}: {}", m_count++, what); }
        size_t count() const noexcept { return m_count; }

        const char* what() const noexcept override { return m_msg.c_str(); }

    private:
        std::string m_msg;
        size_t m_count = 0;
    };

}