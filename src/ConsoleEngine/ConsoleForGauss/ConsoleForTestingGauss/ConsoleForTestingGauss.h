#ifndef PARALLELS_CONSOLEFORTESTINGGAUSS_H
#define PARALLELS_CONSOLEFORTESTINGGAUSS_H

#include "../ConsoleForGauss.h"

namespace s21 {
class ConsoleForTestingGauss : public ConsoleForGauss {
public:
    void SetFileName(std::string filename);
    void RunAlgorithmForTest();
    void RequestParamsFromUserForTest();
    S21Matrix GetResultWithoutUsingParallelism();
    void SetNumberOfRepetitions(int number);
    S21Matrix GetResultUsingParallelism();
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEFORTESTINGGAUSS_H
