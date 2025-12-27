
add_library(epuzzle_core STATIC)

target_sources(epuzzle_core
    PUBLIC
        include/epuzzle/Exceptions.h
        include/epuzzle/Solver.h
        include/epuzzle/PuzzleDefinition.h
        include/epuzzle/PuzzleParser.h
        include/epuzzle/PuzzleSolution.h
        include/epuzzle/SolverConfig.h
    PRIVATE
        src/BruteForce/BruteForceChecker.cpp
        src/BruteForce/BruteForceChecker.h
        src/BruteForce/BruteForceContext.h
        src/BruteForce/BruteForceFactory.cpp
        src/BruteForce/BruteForceFactory.h
        src/BruteForce/BruteForcer.cpp
        src/BruteForce/BruteForcer.h
        src/BruteForce/ICandidateIterator.h
        src/BruteForce/ICandidatesSpace.cpp
        src/BruteForce/ICandidatesSpace.h
        src/BruteForce/IConstraint.cpp
        src/BruteForce/IConstraint.h
        src/BruteForce/ParallelBruteForcer.cpp
        src/BruteForce/ParallelBruteForcer.h
        src/BruteForce/SpaceParallelDistributor.h
        src/Reasoning/Reasoner.cpp
        src/Reasoning/Reasoner.h
        src/IndexedPuzzleData.cpp
        src/IndexedPuzzleData.h
        src/IndexedSolution.cpp
        src/IndexedSolution.h
        src/IndexedTypes.h
        src/Solver.cpp
        src/pch.h
        src/PuzzleDefinition.cpp
        src/PuzzleParser.cpp
        src/PuzzleSolution.cpp
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
