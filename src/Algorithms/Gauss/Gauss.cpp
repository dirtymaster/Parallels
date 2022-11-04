#include "Gauss.h"
namespace s21 {
int Gauss::threads_in_level_;
S21Matrix Gauss::SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices) {
    S21Matrix matrix(matrices[0]), result;
    if (matrix.get_rows() >= 2 && matrix.get_cols() == matrix.get_rows() + 1) {
        double tmp;
        result.set_rows(1);
        result.set_columns(matrix.get_cols() - 1);
        for (int i = 0; i < matrix.get_rows(); ++i) {
            tmp = matrix(i, i);
            for (int j = matrix.get_rows(); j >= i; --j) {
                matrix(i, j) /= tmp;
            }
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                tmp = matrix(j, i);
                for (int k = matrix.get_rows(); k >= i; --k) {
                    matrix(j, k) -= tmp * matrix(i, k);
                }
            }
        }
        result(0, matrix.get_rows() - 1) = matrix(matrix.get_rows() - 1, matrix.get_rows());
        for (int i = matrix.get_rows() - 2; i >= 0; --i) {
            result(0, i) = matrix(i, matrix.get_rows());
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                result(0, i) -= matrix(i, j) * result(0, j);
            }
        }
    }
    return result;
}

S21Matrix Gauss::SolveUsingParallelism(std::vector<S21Matrix> matrices) {
    S21Matrix matrix(matrices[0]), result;
    if (matrix.get_rows() >= 2 && matrix.get_cols() == matrix.get_rows() + 1) {
        threads_in_level_ = matrix.get_rows() < 4 ? matrix.get_rows() : 4;
        double tmp;
        result.set_rows(1);
        result.set_columns(matrix.get_cols() - 1);

        for (int i = 0; i < matrix.get_rows(); ++i) {
            tmp = matrix(i, i);
            FirstLevelParallelization1(matrix, tmp, i);
            SecondLevelParallelization(matrix, tmp, i);
        }
        result(0, matrix.get_rows() - 1) = matrix(matrix.get_rows() - 1, matrix.get_rows());
        for (int i = matrix.get_rows() - 2; i >= 0; --i) {
            result(0, i) = matrix(i, matrix.get_rows());
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                result(0, i) -= matrix(i, j) * result(0, j);
            }
        }
    }
    return result;
}

void Gauss::FirstLevelParallelization1(S21Matrix& matrix, double& tmp, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(DoWork1, std::ref(matrix), std::ref(tmp), i, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::DoWork1(S21Matrix& matrix, double& tmp, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);
    std::mutex mtx;

    for (int j = start_and_end_indices.first[thread_id]; j > start_and_end_indices.second[thread_id]; --j) {
        matrix(i, j) /= tmp;
    }
}

void Gauss::FirstLevelParallelization2(S21Matrix& matrix, double tmp, int i, int j) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(DoWork2, std::ref(matrix), tmp, i, j, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::DoWork2(S21Matrix& matrix, double tmp, int i, int j, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);
    std::mutex mtx;

    for (int k = start_and_end_indices.first[thread_id]; k > start_and_end_indices.second[thread_id]; --k) {
        matrix(j, k) -= tmp * matrix(i, k);
    }
}

void Gauss::SecondLevelParallelization(S21Matrix& matrix, double& tmp, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(DoWork3, std::ref(matrix), std::ref(tmp), i, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::DoWork3(S21Matrix& matrix, double& tmp, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(i + 1, matrix.get_rows(), true);
    std::mutex mtx;

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        mtx.lock();
        tmp = matrix(j, i);
        FirstLevelParallelization2(matrix, tmp, i, j);
        mtx.unlock();
    }
}

std::pair<std::vector<int>, std::vector<int>> Gauss::InitializeStartAndEndIndices(
    int start_index, int end_index, bool start_is_less_than_end) {
    std::vector<int> start_indices(threads_in_level_);
    std::vector<int> end_indices(threads_in_level_);
    start_indices[0] = start_index;
    end_indices[threads_in_level_ - 1] = end_index;
    for (int i = 1; i < threads_in_level_; ++i) {
        if (start_is_less_than_end) {
            end_indices[i - 1] = start_indices[i] =
                start_indices[i - 1] + (double)(end_index - start_index) / threads_in_level_;
        } else {
            end_indices[i - 1] = start_indices[i] =
                start_indices[i - 1] - (double)(start_index - end_index) / threads_in_level_;
        }
    }
    return {start_indices, end_indices};
}

void Gauss::JoinThreads(std::vector<std::thread>& threads) {
    for (int i = 0; i < threads_in_level_; ++i) {
        threads[i].join();
    }
}
}  // namespace s21
