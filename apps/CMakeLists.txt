
add_subdirectory(epuzzle_cli)

if(WIN32)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_samples DESTINATION .)
else()
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/puzzle_samples DESTINATION ${CMAKE_INSTALL_DATADIR}/epuzzle)
endif()

#TODO: install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION .)