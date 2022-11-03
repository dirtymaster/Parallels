cmake_minimum_required(VERSION 3.16)
project(Parallels CXX)

set(CMAKE_CXX_STANDARD 17)

#add_library(decimal_library STATIC src/s21_decimal.c)  # create libdecimal_library.a
#target_link_libraries(decimal_library m) # m - math library

add_executable(a.out ConsoleEngine/main.cpp ConsoleEngine/ConsoleEngine.cpp Algorithms/AbstractAlgorithm.cpp
        Algorithms/Gauss/Gauss.cpp DataStructures/Matrix/Matrix.cpp)
#target_link_libraries(run_tests check decimal_library)