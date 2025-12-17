
add_executable(epuzzle)

target_sources(epuzzle 
    PRIVATE
        src/main.cpp
        src/ProgramOptions.cpp
        src/ProgramOptions.h
)

target_link_libraries(epuzzle 
    PRIVATE 
        cxxopts::cxxopts
        magic_enum::magic_enum
        epuzzle::utils
        epuzzle::core
)    

add_custom_command(TARGET epuzzle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/puzzle_examples
        $<TARGET_FILE_DIR:epuzzle>/puzzle_examples
    COMMENT "Puzzle examples copied"
)

if(WIN32)
    install(TARGETS epuzzle RUNTIME DESTINATION .)
else()
    install(TARGETS epuzzle RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

add_executable(epuzzle::epuzzle ALIAS epuzzle)
