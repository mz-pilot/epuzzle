
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

target_include_directories(epuzzle 
    PRIVATE 
        ${CMAKE_BINARY_DIR}/generated
)

add_custom_target(copy_puzzle_examples ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/puzzle_examples"
        "$<TARGET_FILE_DIR:epuzzle>/puzzle_examples"
)
add_dependencies(epuzzle copy_puzzle_examples)

if(WIN32)
    install(TARGETS epuzzle RUNTIME DESTINATION .)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_examples DESTINATION .)
    install(FILES 
        ${CMAKE_SOURCE_DIR}/docs/README.txt 
        ${CMAKE_SOURCE_DIR}/LICENSE  
        DESTINATION .
    )

else()
    install(TARGETS epuzzle RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_examples DESTINATION ${CMAKE_INSTALL_DATADIR}/epuzzle)
    install(FILES 
        ${CMAKE_SOURCE_DIR}/docs/README.txt 
        ${CMAKE_SOURCE_DIR}/LICENSE 
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
endif()

add_executable(epuzzle::epuzzle ALIAS epuzzle)
