
#ifndef PARALLELS_WINOGRADALGORITHM_H
#define PARALLELS_WINOGRADALGORITHM_H

#include "../AbstractAlgorithm.h"

namespace s21 {

class WinogradAlgorithm : public AbstractAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices);
    S21Matrix SolveUsingParallelism(std::vector <S21Matrix> matrices);

private:
    double *row_factors_;
    double *column_factors_;
    int len_;

    void CalculateRowFactors(S21Matrix &M, start_ind, int end_ind);
    void CalculateColumnFactors(S21Matrix &M, int start_ind, int end_ind);
    void CalculateResultMatrixValues(S21Matrix &res, S21Matrix &M1, S21Matrix &M2,
                                     int start_ind, int end_ind);
    void HandleOddDimension(S21Matrix &res, S21Matrix &M1, S21Matrix &M2,
                            int start_ind, int end_ind);
};

}

#endif //PARALLELS_WINOGRADALGORITHM_H
