#include "AbstractAlgorithm.h"

namespace s21 {
std::pair<double, double> AbstractAlgorithm::MeasureTime(std::vector<S21Matrix> matrices,
                                                   int number_of_repetitions) {
    std::pair<double, double> result;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < number_of_repetitions; ++i) {
        SolveWithoutUsingParallelism(matrices);
    }
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start_time;
    result.first = duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < number_of_repetitions; ++i) {
        SolveUsingParallelism(matrices);
    }
    duration = std::chrono::high_resolution_clock::now() - start_time;
    result.second = duration.count();
    return result;
}
}  // namespace s21
