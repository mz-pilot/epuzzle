
add_library(epuzzle_core STATIC)

target_sources(epuzzle_core
    PUBLIC
        include/epuzzle/Exceptions.h
        include/epuzzle/PuzzleDefinition.h
        include/epuzzle/PuzzleParser.h
        include/epuzzle/PuzzleSolution.h
        include/epuzzle/Solver.h
        include/epuzzle/SolverConfig.h
    PRIVATE
        src/bruteforce/BruteForceContext.h
        src/bruteforce/BruteForceFactory.cpp
        src/bruteforce/BruteForceFactory.h
        src/bruteforce/BruteForcer.cpp
        src/bruteforce/BruteForcer.h
        src/bruteforce/Validator.cpp
        src/bruteforce/Validator.h
        src/bruteforce/ICandidateIterator.h
        src/bruteforce/ICandidatesSpace.cpp
        src/bruteforce/ICandidatesSpace.h
        src/bruteforce/IConstraint.cpp
        src/bruteforce/IConstraint.h
        src/bruteforce/ParallelBruteForcer.cpp
        src/bruteforce/ParallelBruteForcer.h
        src/bruteforce/SpaceParallelDistributor.h
        src/deductive/DeductiveSolver.cpp
        src/deductive/DeductiveSolver.h
        src/IndexedTypes.h
        src/pch.h
        src/PuzzleDefinition.cpp
        src/PuzzleModel.cpp
        src/PuzzleModel.h
        src/PuzzleParser.cpp
        src/PuzzleSolution.cpp
        src/SolutionModel.cpp
        src/SolutionModel.h
        src/Solver.cpp
        src/SolverConfig.cpp
)

target_precompile_headers(epuzzle_core PRIVATE src/pch.h)

target_include_directories(epuzzle_core 
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(epuzzle_core 
    PRIVATE
        tomlplusplus::tomlplusplus
        epuzzle::utils 
)    

add_library(epuzzle::core ALIAS epuzzle_core)
