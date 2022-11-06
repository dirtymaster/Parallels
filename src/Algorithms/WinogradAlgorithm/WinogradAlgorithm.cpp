
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

    CalculateResultMatrixValues(0, M1_.get_rows());

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
//    int nmb_of_threads = 6;
//    std::vector<std::thread> threads(nmb_of_threads);
//
//    for (int i = 0; i < nmb_of_threads; i++) {
//        threads[i] = std::thread(&WinogradAlgorithm::PrepareColumnAndRowFactors, this,
//                                i * M1_.get_rows() / nmb_of_threads, (i + 1) * M1_.get_rows() / nmb_of_threads,
//                                i * M1_.get_cols() / nmb_of_threads, (i + 1) * M2_.get_cols() / nmb_of_threads);
//    }
//
//
//    for (int i = 0; i < nmb_of_threads; i++) {
//        threads[i].join();
//    }
//
//    for (int i = 0; i < nmb_of_threads; i++) {
//        threads[i] = std::thread(&WinogradAlgorithm::CalculateResultMatrixValues, this,
//                                 i * M1_.get_rows() / nmb_of_threads, (i + 1) * M1_.get_rows() / nmb_of_threads);
//    }
//
//    for (int i = 0; i < nmb_of_threads; i++) {
//        threads[i].join();
//    }

    PipelineRealisation();

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
    int M2_cols = M2_.get_cols();
    int M1_cols = M1_.get_cols();
    for (int i = start_ind; i < end_ind; i++) {
        for (int j = 0; j < M2_cols; j++) {
            res_(i, j) += -row_factors_[i] - column_factors_[j];
            for (int k = 0; k < len_; k++) {
                res_(i, j) += (M1_(i , 2*k) + M2_(2*k + 1, j)) * (M1_(i , 2*k + 1) + M2_(2*k, j));
            }
            if (M1_cols % 2 != 0) {
                res_(i, j) += M1_(i, M1_cols - 1) * M2_(M1_cols - 1, j);
            }
        }
    }
}

void WinogradAlgorithm::PrepareColumnAndRowFactors(int start_ind1, int end_ind1,
                                                   int start_ind2, int end_ind2) {
    CalculateRowFactors(start_ind1, end_ind1);
    CalculateColumnFactors(start_ind2, end_ind2);
}

void WinogradAlgorithm::PipelineRealisation() {
    column_factors_ready_ = false;
    row_factors_ready_ = false;
    stage_four_ready_ = false;
    std::thread t1(&WinogradAlgorithm::StageOne, this);
    std::thread t2(&WinogradAlgorithm::StageTwo, this);
    std::thread t3(&WinogradAlgorithm::StageThree, this);
    std::thread t4(&WinogradAlgorithm::StageFour, this);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void WinogradAlgorithm::StageOne() {
    row_factors_mtx_.lock();
    CalculateRowFactors(0, M1_.get_rows());
    row_factors_ready_ = true;
    row_factors_mtx_.unlock();
    row_factors_cv_.notify_all();
}

void WinogradAlgorithm::StageTwo() {
    column_factors_mtx_.lock();
    CalculateColumnFactors(0, M2_.get_cols());
    column_factors_ready_ = true;
    column_factors_mtx_.unlock();
    column_factors_cv_.notify_all();
}

void WinogradAlgorithm::StageThree() {
    std::unique_lock<std::mutex> ul(row_factors_mtx_);
    std::unique_lock<std::mutex> ul2(column_factors_mtx_);
    std::unique_lock<std::mutex> ul3(matrix_mtx_);
    row_factors_cv_.wait(ul, [&] { return row_factors_ready_; });
    column_factors_cv_.wait(ul2, [&] { return column_factors_ready_; });
    matrix_cv_.wait(ul3, [&] { return stage_four_ready_; });

    int cols = res_.get_cols();
    for (int i = 0; i < M1_.get_rows(); i++) {
        for (int j = 0; j < cols; j++) {
            double value = -row_factors_[i] - column_factors_[j];
            for (int k = 0; k < len_; k++) {
                value += (M1_(i, 2 * k) + M2_(2 * k + 1, j)) * (M1_(i, 2 * k + 1) + M2_(2 * k, j));
            }
            res_(i, j) += value;
        }
    }
}

void WinogradAlgorithm::StageFour() {
    matrix_mtx_.lock();
    int cols = res_.get_cols();
    if (M1_.get_cols() % 2 != 0) {
        for (int i = 0; i < M1_.get_rows(); i++) {
            for (int j = 0; j < cols; j++) {
                double value = M1_(i, M1_.get_cols() - 1) * M2_(M1_.get_cols() - 1, j);
                res_(i, j) += value;
            }
        }
    }
    stage_four_ready_ = true;
    matrix_mtx_.unlock();
    matrix_cv_.notify_all();
}

}  // namespace s21
