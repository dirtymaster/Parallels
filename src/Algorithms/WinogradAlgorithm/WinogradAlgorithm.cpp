
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

    PrepareColumnAndRowFactors(0, M1_.get_rows(),
                               0, M2_.get_cols());

    CalculateResult(0, M1_.get_rows());

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
    int nmb_of_threads = 6;
    std::vector<std::thread> threads(nmb_of_threads);

    for (int i = 0; i < nmb_of_threads; i++) {
        threads[i] = std::thread(&WinogradAlgorithm::PrepareColumnAndRowFactors, this,
                                i * M1_.get_rows() / nmb_of_threads, (i + 1) * M1_.get_rows() / nmb_of_threads,
                                i * M1_.get_cols() / nmb_of_threads, (i + 1) * M2_.get_cols() / nmb_of_threads);
    }


    for (int i = 0; i < nmb_of_threads; i++) {
        threads[i].join();
    }

    for (int i = 0; i < nmb_of_threads; i++) {
        threads[i] = std::thread(&WinogradAlgorithm::CalculateResult, this,
                                 i * M1_.get_rows() / nmb_of_threads, (i + 1) * M1_.get_rows() / nmb_of_threads);
    }

    for (int i = 0; i < nmb_of_threads; i++) {
        threads[i].join();
    }

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

void WinogradAlgorithm::PrepareColumnAndRowFactors(int start_ind1, int end_ind1,
                                                   int start_ind2, int end_ind2) {
    CalculateRowFactors(start_ind1, end_ind1);
    CalculateColumnFactors(start_ind2, end_ind2);
}

void WinogradAlgorithm::CalculateResult(int start_ind, int end_ind) {
    CalculateResultMatrixValues(start_ind, end_ind);
    if (M1_.get_cols() % 2 != 0)
        HandleOddDimension(start_ind, end_ind);
}

}  // namespace s21
