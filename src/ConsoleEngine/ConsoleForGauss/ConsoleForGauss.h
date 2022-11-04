#ifndef PARALLELS_CONSOLEFORGAUSS_H
#define PARALLELS_CONSOLEFORGAUSS_H

#include <fstream>

#include "../../Algorithms/Gauss/Gauss.h"
#include "../AbstractConsoleEngine.h"

namespace s21 {
class ConsoleForGauss : public AbstractConsoleEngine {
public:
    ConsoleForGauss();
    ~ConsoleForGauss();

private:
    void RequestParamsFromUser();

    void RunAlgorithm();

    void PrintResult();
    std::string RequestFilenameFromUser();
    S21Matrix RequestMatrixFromUser();
    int RequestNumberOfRepetitions();
    void PrintMatrix(S21Matrix matrix);
    void GenerateRandomMatrix();

    AbstractAlgorithm *gauss_algorithm_;
    S21Matrix matrix_;
    int number_of_repetitions_;
    S21Matrix result_without_using_parallelism_;
    S21Matrix result_using_parallelism_;
    std::pair<double, double> times_;
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEFORGAUSS_H
