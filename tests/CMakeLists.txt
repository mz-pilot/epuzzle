
add_executable(epuzzle_tests)

set_target_properties(epuzzle_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests")

target_sources(epuzzle_tests
    PRIVATE
        src/BruteForceTests.cpp
        src/ISolverRealPuzzleTests.cpp
        src/ISolverTests.cpp
        src/pch.h
        src/PuzzleDataLoaderTests.cpp
        src/PuzzleDataTests.cpp
        src/TestUtils.cpp
        src/TestUtils.h
)

target_precompile_headers(epuzzle_tests PRIVATE src/pch.h)

target_compile_definitions(epuzzle_tests PRIVATE EPUZZLE_TEST_SAMPLES_DIR="${CMAKE_SOURCE_DIR}/puzzle_samples")

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt $<NOT:${EPUZZLE_STATIC_RUNTIME}>)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

target_link_libraries(epuzzle_tests 
    PRIVATE
        GTest::gtest_main
        epuzzle::utils
        epuzzle::core
)    

target_include_directories(epuzzle_tests 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/core/src
)

gtest_discover_tests(epuzzle_tests EXTRA_ARGS "--gtest_color=yes")

add_executable(epuzzle::tests ALIAS epuzzle_tests)