#include "AbstractAlgorithm.h"
namespace s21 {
std::pair<double, double> AbstractAlgorithm::MeasureTime(std::vector<S21Matrix> matrices,
                                                         std::pair<S21Matrix, S21Matrix>& results,
                                                         int number_of_repetitions) {
    std::pair<double, double> times;
    auto start_time = std::chrono::high_resolution_clock::now();
    results.first = SolveWithoutUsingParallelism(matrices);  // записываем результат работы
    for (int i = 1; i < number_of_repetitions; ++i) {
        SolveWithoutUsingParallelism(matrices);
    }
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start_time;
    times.first = duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    results.second = SolveUsingParallelism(matrices);  // записываем результат работы
    for (int i = 1; i < number_of_repetitions; ++i) {
        SolveUsingParallelism(matrices);
    }
    duration = std::chrono::high_resolution_clock::now() - start_time;
    times.second = duration.count();
    return times;
}
}  // namespace s21
