
add_library(epuzzle_utils STATIC)

target_sources(epuzzle_utils
    PUBLIC
        include/utils/Concurrency.h
        include/utils/Diagnostics.h
        include/utils/Hardware.h
        include/utils/Math.h
        include/utils/Meta.h
        include/utils/platform/ConsoleUtils.h
        include/utils/Progress.h
        include/utils/Text.h
    PRIVATE
        src/platform/ConsoleUtils.cpp
)

target_include_directories(epuzzle_utils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_library(epuzzle::utils ALIAS epuzzle_utils)
