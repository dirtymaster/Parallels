
#include "WinogradAlgorithm.h"

namespace s21 {

S21Matrix WinogradAlgorithm::SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
        exit(1);
    }

    if (matrices[0].get_cols() != matrices[1].get_rows()) {
        printf("get cols != get rows\n");
        exit(2);
    }

    S21Matrix result(matrices[0].get_rows(), matrices[1].get_cols());
    S21Matrix &G = matrices[0];
    S21Matrix &H = matrices[1];
    int d = G.get_cols() / 2;

    double *row_factors = new double[G.get_rows()];

    for (int i = 0; i < G.get_rows(); i++) {
        row_factors[i] = G(i, 0) * G(i , 1);
        for (int j = 1; j < d; j++) {
            row_factors[i] += G(i, 2 * j) * G(i, 2 * j + 1);
        }
    }

    double *column_factors = new double[H.get_cols()];

    for (int i = 0; i < H.get_cols(); i++) {
        column_factors[i] = H(0, i) * H(1, i);
        for (int j = 1; j < d; j++) {
            column_factors[i] += H(2 * j, i) * H(2 * j + 1, i);
        }
    }

    for (int i = 0; i < G.get_rows(); i++) {
        for (int j = 0; j < H.get_cols(); j++) {
            result(i, j) = -row_factors[i] - column_factors[j];
            for (int k = 0; k < d; k++) {
                result(i, j) += (G(i , 2*k) + H(2*k + 1, j)) * (G(i , 2*k + 1) + H(2*k, j));
            }
        }
    }

    if (G.get_cols() % 2 != 0) {
        for (int i = 0; i < G.get_rows(); i++) {
            for (int j = 0; j < H.get_cols(); j++) {
                result(i, j) += G(i, G.get_cols() - 1) * H(G.get_cols() - 1, j);
            }
        }
    }

    delete[] row_factors;
    delete[] column_factors;
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

    CalculateRowFactors(0, M1.get_rows());
    CalculateColumnFactors(0, M2.get_cols());
    CalculateResultMatrixValues(result, 0, M1.get_rows());

    delete[] row_factors_;
    delete[] column_factors_;
    M1 = nullptr;
    M2 = nullptr;
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
            result(i, j) += M1(i, M1.get_cols() - 1) * M2(M1.get_cols() - 1, j);
        }
    }
}

}  // namespace s21
