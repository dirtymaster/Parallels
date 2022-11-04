#ifndef A3_PARALLELS_0_MASTER_GAUSS_H
#define A3_PARALLELS_0_MASTER_GAUSS_H

#include <iostream>
#include <mutex>
#include <thread>

#include "../AbstractAlgorithm.h"

namespace s21 {
class Gauss : public AbstractAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices) override;
    S21Matrix SolveUsingParallelism(std::vector<S21Matrix> matrices) override;

private:
    static int threads_in_level_;
    static void FirstLevelParallelization1(S21Matrix& matrix, double& tmp, int i);
    static void DoWork1(S21Matrix& matrix, double& tmp, int i, int thread_id);
    static void FirstLevelParallelization2(S21Matrix& matrix, double tmp, int i, int j);
    static void DoWork2(S21Matrix& matrix, double tmp, int i, int j, int thread_id);
    static void SecondLevelParallelization(S21Matrix& matrix, double& tmp, int i);
    static void DoWork3(S21Matrix& matrix, double& tmp, int i, int thread_id);
    static std::pair<std::vector<int>, std::vector<int>> InitializeStartAndEndIndices(
        int start_index, int end_index, bool start_is_less_than_end);
    static void JoinThreads(std::vector<std::thread>& threads);
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_GAUSS_H
