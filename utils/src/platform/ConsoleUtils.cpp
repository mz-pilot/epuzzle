#include <locale>
#include "utils/platform/ConsoleUtils.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace platform
{
	void setupConsole()
	{
#ifdef _WIN32
		::SetConsoleCP(CP_UTF8);
		::SetConsoleOutputCP(CP_UTF8);
		std::locale::global(std::locale(".UTF-8"));
#endif
	}
}