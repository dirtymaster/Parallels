#ifndef PARALLELS_CONSOLEENGINE_H
#define PARALLELS_CONSOLEENGINE_H

#include <fstream>
#include <string>

#include "../Algorithms/Gauss/Gauss.h"

namespace s21 {
class ConsoleEngine {
public:
    ~ConsoleEngine();
    void start();

private:
    AbstractAlgorithm* abstract_algorithm_ = nullptr;

    enum MenuOptions { EXIT = 0, ANT_ALGORITHM = 1, GAUSS_ALGORITHM = 2, WINOGRAD_ALGORITHM = 3 };
    enum UseParallelismOrNot { USE = 1, DO_NOT_USE = 2 };
    std::string menu_options_ =
        ("Menu options:\n"
         "\t 1. Solve salesman problem using and algorithm.\n"
         "\t 2. Solve SLAE using the Gauss method.\n"
         "\t 3. Multiply matrices using the Winograd algorithm\n"
         "\t 0. Close the application.\n");

    S21Matrix RequestMatrixFromUser();
    const std::string RequestFilenameFromUser();
    void PrintMatrix(S21Matrix matrix);
    int RequestUseParallelismOrNot();
    std::string use_parallelism_or_not_ =
        ("Solve using parallelism?\n"
         "\t1. Use parallelism.\n"
         "\t2. Do not use parallelism.\n");
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEENGINE_H
