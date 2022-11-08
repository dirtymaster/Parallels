#include "ConsoleForTestingGauss.h"

namespace s21 {
void ConsoleForTestingGauss::SetFileName(std::string filename) { filename_ = filename; }

void ConsoleForTestingGauss::RunAlgorithmForTest() { RunAlgorithm(); }

void ConsoleForTestingGauss::RequestParamsFromUserForTest() { RequestParamsFromUser(); }

S21Matrix ConsoleForTestingGauss::GetResultWithoutUsingParallelism() {
    return result_without_using_parallelism_;
}

void ConsoleForTestingGauss::SetNumberOfRepetitions(int number) { number_of_repetitions_ = number; }

S21Matrix ConsoleForTestingGauss::GetResultUsingParallelism() { return result_using_parallelism_; }

}  // namespace s21