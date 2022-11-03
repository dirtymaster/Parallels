#ifndef A3_PARALLELS_0_MASTER_ABSTRACT_ALGORITHM_H
#define A3_PARALLELS_0_MASTER_ABSTRACT_ALGORITHM_H

#include <vector>
#include "time.h"

#include "../DataStructures/Matrix/Matrix.h"

namespace s21 {
class AbstractAlgorithm {
public:
    virtual ~AbstractAlgorithm() {}
    virtual S21Matrix SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices) = 0;
    virtual S21Matrix SolveUsingParallelism(std::vector<S21Matrix> matrices) = 0;
    std::pair<int, int> MeasureTime(
        std::vector<S21Matrix> matrices,
        int number_of_repetitions);  //  возвращает время решения без параллелизма и с
};
}  // namespace s21

#endif  // A3_PARALLELS_0_MASTER_ABSTRACT_ALGORITHM_H
