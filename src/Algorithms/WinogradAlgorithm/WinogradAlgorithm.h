
#ifndef PARALLELS_WINOGRADALGORITHM_H
#define PARALLELS_WINOGRADALGORITHM_H

#include <thread>
#include "../AbstractAlgorithm.h"

namespace s21 {

class WinogradAlgorithm : public AbstractAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices);
    S21Matrix SolveUsingParallelism(std::vector <S21Matrix> matrices);

private:
    double *row_factors_;
    double *column_factors_;
    S21Matrix M1_;
    S21Matrix M2_;
    S21Matrix res_;
    int len_;

    void CalculateRowFactors(int start_ind, int end_ind);
    void CalculateColumnFactors(int start_ind, int end_ind);
    void CalculateResultMatrixValues(int start_ind, int end_ind);
    void HandleOddDimension(int start_ind, int end_ind);
    void CalculatePartOfMatrix(int start_ind1, int end_ind1,
                               int start_ind2, int end_ind2);
};

}

#endif //PARALLELS_WINOGRADALGORITHM_H
