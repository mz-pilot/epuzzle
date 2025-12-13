
add_executable(epuzzle_cli)

target_sources(epuzzle_cli
    PRIVATE
        src/main.cpp
        src/ProgramOptions.cpp
        src/ProgramOptions.h
)

target_link_libraries(epuzzle_cli 
    PRIVATE 
        cxxopts::cxxopts
        magic_enum::magic_enum
        epuzzle::utils
        epuzzle::core
)    

add_custom_command(TARGET epuzzle_cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/puzzle_samples
        $<TARGET_FILE_DIR:epuzzle_cli>/puzzle_samples
    COMMENT "Puzzle samples copied"
)

if(WIN32)
    install(TARGETS epuzzle_cli RUNTIME DESTINATION .)
else()
    install(TARGETS epuzzle_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

add_executable(epuzzle::epuzzle_cli ALIAS epuzzle_cli)
