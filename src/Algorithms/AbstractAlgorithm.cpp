#include "AbstractAlgorithm.h"

namespace s21 {
std::pair<int, int> AbstractAlgorithm::MeasureTime(std::vector<S21Matrix> matrices, int number_of_repetitions) {
    std::pair<int, int> result;
    unsigned int start_time = clock();
    for (int i = 0; i < number_of_repetitions; ++i) {
        SolveWithoutUsingParallelism(matrices);
    }
    result.first = (clock() - start_time) / CLOCKS_PER_SEC;  // без параллелизма

    start_time = clock();
    for (int i = 0; i < number_of_repetitions; ++i) {
        SolveUsingParallelism(matrices);
    }
    result.second = (clock() - start_time) / CLOCKS_PER_SEC;  // без параллелизма
    return result;
}
}  // namespace s21
