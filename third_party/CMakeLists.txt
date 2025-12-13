
# Locally vendorized header-only third party libraries

set(EPUZZLE_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE $<BUILD_INTERFACE:${EPUZZLE_THIRD_PARTY_DIR}>)
add_library(cxxopts::cxxopts ALIAS cxxopts)

add_library(magic_enum INTERFACE)
target_include_directories(magic_enum INTERFACE $<BUILD_INTERFACE:${EPUZZLE_THIRD_PARTY_DIR}>)
add_library(magic_enum::magic_enum ALIAS magic_enum)

add_library(tomlplusplus INTERFACE)
target_include_directories(tomlplusplus INTERFACE $<BUILD_INTERFACE:${EPUZZLE_THIRD_PARTY_DIR}>)
add_library(tomlplusplus::tomlplusplus ALIAS tomlplusplus)
