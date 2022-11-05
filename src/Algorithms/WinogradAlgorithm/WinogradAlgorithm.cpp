
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

    M1_ = matrices[0];
    M2_ = matrices[1];
    res_ = S21Matrix(M1_.get_rows(), M2_.get_cols());
    row_factors_ = new double[M1_.get_rows()];
    column_factors_ = new double[M2_.get_cols()];
    len_ = M1_.get_cols() / 2;

    CalculatePartOfMatrix(0, M1_.get_rows(), 0, M2_.get_cols());

    delete[] row_factors_;
    delete[] column_factors_;

    return res_;
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

    M1_ = matrices[0];
    M2_ = matrices[1];
    res_ = S21Matrix(M1_.get_rows(), M2_.get_cols());
    row_factors_ = new double[M1_.get_rows()];
    column_factors_ = new double[M2_.get_cols()];
    len_ = M1_.get_cols() / 2;

    std::thread t1(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   0, M1_.get_rows() / 6, 0, M2_.get_cols() / 6);
    std::thread t2(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   M1_.get_rows() / 6, 2 * M1_.get_rows() / 6, M2_.get_cols() / 6, 2 * M2_.get_cols() / 6);
    std::thread t3(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   2 * M1_.get_rows() / 6, 3 * M1_.get_rows() / 6, 2 * M2_.get_cols() / 6, 3 * M2_.get_cols() / 6);
    std::thread t4(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   3 * M1_.get_rows() / 6, 4 * M1_.get_rows() / 6, 3 * M2_.get_cols() / 6, 4 * M2_.get_cols() / 6);
    std::thread t5(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   4 * M1_.get_rows() / 6, 5 * M1_.get_rows() / 6, 4 * M2_.get_cols() / 6, 5 * M2_.get_cols() / 6);
    std::thread t6(&WinogradAlgorithm::CalculatePartOfMatrix, this,
                   5 * M1_.get_rows() / 6, 6 * M1_.get_rows() / 6, 5 * M2_.get_cols() / 6, 6 * M2_.get_cols() / 6);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    delete[] row_factors_;
    delete[] column_factors_;

    return res_;
}

void WinogradAlgorithm::CalculateRowFactors(int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        row_factors_[i] = M1_(i, 0) * M1_(i , 1);
        for (int j = 1; j < len_; j++) {
            row_factors_[i] += M1_(i, 2 * j) * M1_(i, 2 * j + 1);
        }
    }
}

void WinogradAlgorithm::CalculateColumnFactors(int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        column_factors_[i] = M2_(0, i) * M2_(1, i);
        for (int j = 1; j < len_; j++) {
            column_factors_[i] += M2_(2 * j, i) * M2_(2 * j + 1, i);
        }
    }
}

void WinogradAlgorithm::CalculateResultMatrixValues(int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        for (int j = 0; j < M2_.get_cols(); j++) {
            res_(i, j) = -row_factors_[i] - column_factors_[j];
            for (int k = 0; k < len_; k++) {
                res_(i, j) += (M1_(i , 2*k) + M2_(2*k + 1, j)) * (M1_(i , 2*k + 1) + M2_(2*k, j));
            }
        }
    }
}

void WinogradAlgorithm::HandleOddDimension(int start_ind, int end_ind) {
    for (int i = start_ind; i < end_ind; i++) {
        for (int j = 0; j < M2_.get_cols(); j++) {
            res_(i, j) += M1_(i, M1_.get_cols() - 1) * M2_(M1_.get_cols() - 1, j);
        }
    }
}

void WinogradAlgorithm::CalculatePartOfMatrix(int start_ind1, int end_ind1,
                                                int start_ind2, int end_ind2) {
    CalculateRowFactors(start_ind1, end_ind1);
    CalculateColumnFactors(start_ind2, end_ind2);
    CalculateResultMatrixValues(start_ind1, end_ind1);
    if (M1_.get_cols() % 2 != 0)
        HandleOddDimension(start_ind1, end_ind1);


}

}  // namespace s21
