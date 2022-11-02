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
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_SLAE_H
