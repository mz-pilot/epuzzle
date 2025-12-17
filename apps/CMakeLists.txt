
add_subdirectory(epuzzle)

if(WIN32)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_examples DESTINATION .)
else()
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_examples DESTINATION ${CMAKE_INSTALL_DATADIR}/epuzzle)
endif()

#TODO: install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION .)