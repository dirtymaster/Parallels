#ifndef A3_PARALLELS_0_MASTER_GAUSS_H
#define A3_PARALLELS_0_MASTER_GAUSS_H

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "../../DataStructures/Matrix/Matrix.h"

using std::thread;
using std::vector;

namespace s21 {
class GaussAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(S21Matrix matrix);
    S21Matrix SolveUsingParallelism(S21Matrix matrix);
    std::pair<double, double> MeasureTime(S21Matrix matrix, std::pair<S21Matrix, S21Matrix>& results,
                                          int number_of_repetitions);

private:
    static int threads_in_level_;
    static void DivideEquation(S21Matrix& matrix, double tmp, int i);
    static void DivideEquationCycle(S21Matrix& matrix, double tmp, int i, int thread_id);
    static void SubtractElementsInMatrix(S21Matrix& matrix, int i);
    static void SubtractElementsInMatrixCycle(S21Matrix& matrix, int i, int thread_id);
    static void EquateResultsToRightValues(S21Matrix& matrix, S21Matrix& result);
    static void EquateResultsToRightValuesCycle(S21Matrix& matrix, S21Matrix& result, int thread_id);
    static void SubtractCalculatedVariables(S21Matrix& matrix, S21Matrix& result, int i);
    static void SubtractCalculatedVariablesCycle(S21Matrix& matrix, S21Matrix& result, int i, int thread_id,
                                                 std::mutex& mtx);
    static std::pair<std::vector<int>, std::vector<int>> InitializeStartAndEndIndices(
        int start_index, int end_index, bool start_is_less_than_end);
    static void JoinThreads(std::vector<std::thread>& threads);
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_GAUSS_H
