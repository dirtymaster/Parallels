cmake_minimum_required(VERSION 3.16)
project(Parallels CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(a.out ConsoleEngine/ConsoleForGauss/main.cpp ConsoleEngine/ConsoleForGauss/ConsoleForGauss.cpp
        Algorithms/GaussAlgorithm/GaussAlgorithm.cpp DataStructures/Matrix/Matrix.cpp)
#target_link_libraries(run_tests check decimal_library)