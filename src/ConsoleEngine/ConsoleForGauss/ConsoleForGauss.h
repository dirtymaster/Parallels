#ifndef PARALLELS_CONSOLEFORGAUSS_H
#define PARALLELS_CONSOLEFORGAUSS_H

#include <fstream>
#include <string>

#include "../../Algorithms/GaussAlgorithm/GaussAlgorithm.h"
#include "../AbstractConsoleEngine.h"

using std::cout;
using std::cin;
using std::endl;

namespace s21 {
class ConsoleForGauss : public AbstractConsoleEngine {
public:
    ConsoleForGauss();
    ~ConsoleForGauss();

private:
    void PrintResult();
    void RequestFilenameFromUser();
    S21Matrix RequestMatrixFromUser();
    int RequestNumberOfRepetitions();
    void PrintMatrix(S21Matrix matrix);
    void GenerateRandomMatrix();

    GaussAlgorithm *gauss_algorithm_;
    S21Matrix matrix_;
    std::pair<double, double> times_;
    std::pair<S21Matrix, S21Matrix> results_;

protected:
    void RequestParamsFromUser();
    void RunAlgorithm();
    std::string filename_;
    S21Matrix result_without_using_parallelism_;
    int number_of_repetitions_ = -1;
    S21Matrix result_using_parallelism_;
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEFORGAUSS_H
