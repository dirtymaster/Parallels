#ifndef A3_PARALLELS_0_MASTER_GAUSS_H
#define A3_PARALLELS_0_MASTER_GAUSS_H

#include <iostream>
#include <mutex>
#include <thread>

#include "../AbstractAlgorithm.h"

namespace s21 {
class Gauss : public AbstractAlgorithm {
public:
    Gauss() {}
    S21Matrix SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices);
    S21Matrix SolveUsingParallelism(std::vector<S21Matrix> matrices);
    std::vector<int> MeasureTime() { return std::vector<int>(); }
    void PrintResults(std::vector<int> times) {}

private:
    static int levels_;
    static int threads_per_level_;
    static void FirstLevelParallelization(S21Matrix& matrix, double& tmp, int i, std::vector<std::thread*> threads);
    static void FirstLevelParallelization2(S21Matrix& matrix, double tmp, int i, int j,
                                        std::vector<std::thread*> threads);
    static void SecondLevelParallelization(S21Matrix& matrix, double& tmp, int i, std::vector<std::thread*> threads);
    static void ThirdLevelParallelization(S21Matrix& matrix, double& tmp, std::vector<std::thread*> threads);
    static void DeleteThreadsFromTo(std::vector<std::thread*>& threads, int level);
    static void JoinThreadsFromTo(std::vector<std::thread*>& threads, int level);
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_SLAE_H
