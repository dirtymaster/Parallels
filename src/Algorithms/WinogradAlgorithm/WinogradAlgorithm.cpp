
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
    }

    if (matrices[0].get_cols() != matrices[1].get_rows()) {

    }

    S21Matrix result;

    return result;
}



}  // namespace s21