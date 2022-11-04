#include "Gauss.h"
namespace s21 {
int Gauss::number_of_threads_ = 2;
std::vector<std::thread*> Gauss::threads_ = std::vector<std::thread*>(number_of_threads_);
S21Matrix Gauss::SolveWithoutUsingParallelism(std::vector<S21Matrix> matrices) {
    S21Matrix matrix(matrices[0]), result;
    if (matrix.get_rows() >= 2 || matrix.get_cols() >= 3) {
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
    if (matrix.get_rows() >= 2 || matrix.get_cols() >= 3) {
        double tmp;
        result.set_rows(1);
        result.set_columns(matrix.get_cols() - 1);

        for (int i = 0; i < matrix.get_rows(); ++i) {
            tmp = matrix(i, i);
            FirstLevelParallelization1(matrix, tmp, i);
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                tmp = matrix(j, i);
                FirstLevelParallelization2(matrix, tmp, i, j);
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

void Gauss::FirstLevelParallelization1(S21Matrix& matrix, double& tmp, int i) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork1, std::ref(matrix), std::ref(tmp), i, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork1(S21Matrix& matrix, double& tmp, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);

    for (int j = start_and_end_indices.first[thread_id]; j > start_and_end_indices.second[thread_id]; --j) {
        matrix(i, j) /= tmp;
    }
}

void Gauss::FirstLevelParallelization2(S21Matrix& matrix, double tmp, int i, int j) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork2, std::ref(matrix), tmp, i, j, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork2(S21Matrix& matrix, double tmp, int i, int j, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);

    for (int k = start_and_end_indices.first[thread_id]; k > start_and_end_indices.second[thread_id]; --k) {
        matrix(j, k) -= tmp * matrix(i, k);
    }
}

void Gauss::FirstLevelParallelization3(S21Matrix& matrix, S21Matrix& result, int i) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork3, std::ref(matrix), std::ref(result), i, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork3(S21Matrix& matrix, S21Matrix& result, int i, int thread_id) {
    //    for (int j = i + 1; j < matrix.get_rows(); ++j) {
    //        result(0, i) -= matrix(i, j) * result(0, j);
    //    }
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(i + 1, matrix.get_rows(), true);

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        result(0, i) -= matrix(i, j) * result(0, j);
    }
}

std::pair<std::vector<int>, std::vector<int>> Gauss::InitializeStartAndEndIndices(
    int start_index0, int end_index1, bool start_is_less_than_end) {
    std::vector<int> start_indices(number_of_threads_);
    std::vector<int> end_indices(number_of_threads_);
    start_indices[0] = start_index0;
    end_indices[1] = end_index1;
    if (start_is_less_than_end) {
        end_indices[0] = start_indices[1] =
            start_indices[0] + (double)(end_indices[1] - start_indices[0]) / number_of_threads_;
    } else {
        end_indices[0] = start_indices[1] =
            start_indices[0] - (double)(start_indices[0] - end_indices[1]) / number_of_threads_;
    }
    return {start_indices, end_indices};
}

void Gauss::DeleteThreadsFromTo() {
    for (int i = 0; i < number_of_threads_; ++i) {
        delete threads_[i];
    }
}

void Gauss::JoinThreadsFromTo() {
    for (int i = 0; i < number_of_threads_; ++i) {
        threads_[i]->join();
    }
}
}  // namespace s21
