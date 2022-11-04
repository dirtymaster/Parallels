
#include "WinogradAlgorithm.h"

namespace s21 {

S21Matrix WinogradAlgorithm::SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
        exit(1);
    }
    if (matrices[0].get_cols() != matrices[1].get_rows()) {
        printf("Wrong matrix dimensions\n");
        exit(2);
    }

    S21Matrix &M1 = matrices[0];
    S21Matrix &M2 = matrices[1];

    S21Matrix result(M1.get_rows(), M2.get_cols());
    row_factors_ = new double[M1.get_rows()];
    column_factors_ = new double[M2.get_cols()];
    len_ = M1.get_cols() / 2;

    CalculateRowFactors(M1, 0, M1.get_rows());
    CalculateColumnFactors(M2, 0, M2.get_cols());
    CalculateResultMatrixValues(result, M1, M2, 0, M1.get_rows());

    if (M1.get_cols() % 2 != 0) {
        HandleOddDimension(result, M1, M2, 0, M1.get_rows());
    }

    delete[] row_factors_;
    delete[] column_factors_;

    return result;
}

S21Matrix WinogradAlgorithm::SolveUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
        exit(1);
    }
    if (matrices[0].get_cols() != matrices[1].get_rows()) {
        printf("Wrong matrix dimensions\n");
        exit(2);
    }

    S21Matrix &M1 = matrices[0];
    S21Matrix &M2 = matrices[1];

    S21Matrix result(M1.get_rows(), M2.get_cols());
    row_factors_ = new double[M1.get_rows()];
    column_factors_ = new double[M2.get_cols()];
    len_ = M1.get_cols() / 2;
    std::thread t1(&WinogradAlgorithm::CalculatePartOfMatrix, this, std::ref(result),
                   std::ref(M1), std::ref(M2), 0, M1.get_rows() / 2, 0, M2.get_cols() / 2);
    std::thread t2(&WinogradAlgorithm::CalculatePartOfMatrix, this, std::ref(result),
                   std::ref(M1), std::ref(M2), M1.get_rows() / 2, M1.get_rows(), M2.get_cols() / 2, M2.get_cols());

    //CalculatePartOfMatrix(result, M1, M2, 0, M1.get_rows(), M2.get_cols());
//    CalculateRowFactors(M1, 0, M1.get_rows());
//    CalculateColumnFactors(M2, 0, M2.get_cols());
//    CalculateResultMatrixValues(result, M1, M2, 0, M1.get_rows());
//
//    if (M1.get_cols() % 2 != 0) {
//        HandleOddDimension(result, M1, M2, 0, M1.get_rows());
//    }
    t1.join();
    t2.join();

    delete[] row_factors_;
    delete[] column_factors_;

    return result;
}

void WinogradAlgorithm::CalculateRowFactors(S21Matrix &M, int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        row_factors_[i] = M(i, 0) * M(i , 1);
        for (int j = 1; j < len_; j++) {
            row_factors_[i] += M(i, 2 * j) * M(i, 2 * j + 1);
        }
    }
}

void WinogradAlgorithm::CalculateColumnFactors(S21Matrix &M, int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        column_factors_[i] = M(0, i) * M(1, i);
        for (int j = 1; j < len_; j++) {
            column_factors_[i] += M(2 * j, i) * M(2 * j + 1, i);
        }
    }
}

void WinogradAlgorithm::CalculateResultMatrixValues(S21Matrix &res,S21Matrix &M1, S21Matrix &M2,
                                                    int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        for (int j = 0; j < M2.get_cols(); j++) {
            res(i, j) = -row_factors_[i] - column_factors_[j];
            for (int k = 0; k < len_; k++) {
                res(i, j) += (M1(i , 2*k) + M2(2*k + 1, j)) * (M1(i , 2*k + 1) + M2(2*k, j));
            }
        }
    }
}

void WinogradAlgorithm::HandleOddDimension(S21Matrix &res, S21Matrix &M1, S21Matrix &M2,
                                           int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        for (int j = 0; j < M2.get_cols(); j++) {
            res(i, j) += M1(i, M1.get_cols() - 1) * M2(M1.get_cols() - 1, j);
        }
    }
}

void WinogradAlgorithm::CalculatePartOfMatrix(S21Matrix &res, S21Matrix &M1, S21Matrix &M2, int start_ind,
                                              int end_ind, int start2, int end2) {
    CalculateRowFactors(M1, start_ind, end_ind);
    CalculateColumnFactors(M2, start2, end2);
    CalculateResultMatrixValues(res, M1, M2, start_ind, end_ind);
    if (M1.get_cols() % 2 != 0)
        HandleOddDimension(res, M1, M2, start_ind, end_ind);
}

}  // namespace s21
