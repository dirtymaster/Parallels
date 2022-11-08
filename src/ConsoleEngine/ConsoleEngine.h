#ifndef PARALLELS_CONSOLEENGINE_H
#define PARALLELS_CONSOLEENGINE_H

#include <fstream>
#include <string>

#include "../Algorithms/Gauss/GaussAlgorithm.h"

namespace s21 {
class ConsoleEngine {
public:
    ~ConsoleEngine();
    void start();

private:
    AbstractAlgorithm* abstract_algorithm_ = nullptr;

    enum MenuOptions { EXIT = 0, ANT_ALGORITHM = 1, GAUSS_ALGORITHM = 2, WINOGRAD_ALGORITHM = 3 };
    enum UseParallelismOrNot { USE = 1, DO_NOT_USE = 2 };
    static std::string menu_options_;

    static S21Matrix RequestMatrixFromUser();
    static std::string RequestFilenameFromUser();
    static void PrintMatrix(S21Matrix matrix);
    int RequestNumberOfRepetitions();
    void PrintResults();
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEENGINE_H
