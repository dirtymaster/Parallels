cmake_minimum_required(VERSION 3.16)
project(Parallels CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
include_directories(${gtest_SOURCES_DIR}/include ${gtest_SOURCES_DIR})
add_executable(test.out Tests/Tests.cpp ConsoleEngine/ConsoleForGauss/main.cpp ConsoleEngine/ConsoleForGauss/ConsoleForGauss.cpp
        ConsoleEngine/ConsoleForGauss/ConsoleForTestingGauss/ConsoleForTestingGauss.cpp
        ConsoleEngine/ConsoleForAnt/ConsoleForAnt.cpp ConsoleEngine/ConsoleForWinograd/ConsoleForWinograd.cpp
        Algorithms/AntColonyAlgorithm/AntAlgorithm.cpp Algorithms/WinogradAlgorithm/WinogradAlgorithm.cpp
        Algorithms/GaussAlgorithm/GaussAlgorithm.cpp DataStructures/Matrix/Matrix.cpp)
target_link_libraries(test.out gtest gtest_main)