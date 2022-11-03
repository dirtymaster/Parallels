#ifndef A3_PARALLELS_0_MASTER_GAUSS_H
#define A3_PARALLELS_0_MASTER_GAUSS_H

#include <iostream>
#include <thread>

#include "../AbstractAlgorithm.h"

namespace s21 {
class Gauss : public AbstractAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices) override;
    S21Matrix SolveUsingParallelism(std::vector<S21Matrix> matrices) override;

private:
    static int number_of_threads_;
    static std::vector<std::thread*> threads_;
    static void Parallelization1(S21Matrix& matrix, double& tmp, int i);
    static void DoWork1(S21Matrix& matrix, double& tmp, int i, int thread_id);
    static void Parallelization2(S21Matrix& matrix, double tmp, int i, int j);
    static void DoWork2(S21Matrix& matrix, double tmp, int i, int j, int thread_id);
    static void Parallelization3(S21Matrix& matrix, S21Matrix& result, int i);
    static void DoWork3(S21Matrix& matrix, S21Matrix& result, int i, int thread_id);
    static void DeleteThreadsFromTo();
    static void JoinThreadsFromTo();
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_GAUSS_H
