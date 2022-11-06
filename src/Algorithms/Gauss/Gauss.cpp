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
        threads_in_level_ = 2;
        double tmp;
        result.set_rows(1);
        result.set_columns(matrix.get_cols() - 1);

        int rows = matrix.get_rows();
        for (int i = 0; i < rows; ++i) {
            tmp = matrix(i, i);
            DivideEquation(matrix, tmp, i);
            SubtractElementsInMatrix(matrix, i);
        }
        result(0, rows - 1) = matrix(rows - 1, rows);
        EquateResultsToRightValues(matrix, result);
        for (int i = rows - 2; i >= 0; --i) {
            SubtractCalculatedVariables(matrix, result, i);
        }
    }
    return result;
}

void Gauss:: DivideEquation(S21Matrix& matrix, double tmp, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(DivideEquationCycle, std::ref(matrix), tmp, i, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::DivideEquationCycle(S21Matrix& matrix, double tmp, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);

    for (int j = start_and_end_indices.first[thread_id]; j > start_and_end_indices.second[thread_id]; --j) {
        matrix(i, j) /= tmp;
    }
}

void Gauss::SubtractElementsInRow(S21Matrix& matrix, double tmp, int i, int j) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(SubtractElementsInRowCycle, std::ref(matrix), tmp, i, j, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::SubtractElementsInRowCycle(S21Matrix& matrix, double tmp, int i, int j, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);

    for (int k = start_and_end_indices.first[thread_id]; k > start_and_end_indices.second[thread_id]; --k) {
        matrix(j, k) -= tmp * matrix(i, k);
    }
}

void Gauss::SubtractElementsInMatrix(S21Matrix& matrix, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(SubtractElementsInMatrixCycle, std::ref(matrix), i, thread_id);
    }
    JoinThreads(threads);
}

void Gauss::SubtractElementsInMatrixCycle(S21Matrix& matrix, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(i + 1, matrix.get_rows(), true);

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        SubtractElementsInRow(matrix, matrix(j, i), i, j);
    }
}

void Gauss::EquateResultsToRightValues(s21::S21Matrix& matrix, s21::S21Matrix& result) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(EquateResultsToRightValuesCycle, std::ref(matrix), std::ref(result), thread_id);
    }
    JoinThreads(threads);
}

void Gauss::EquateResultsToRightValuesCycle(S21Matrix& matrix, S21Matrix& result, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows() - 2, -1, false);

    for (int i = start_and_end_indices.first[thread_id]; i > start_and_end_indices.second[thread_id]; --i) {
        result(0, i) = matrix(i, matrix.get_rows());
    }
}

void Gauss::SubtractCalculatedVariables(s21::S21Matrix& matrix, s21::S21Matrix& result, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    std::mutex mtx;
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] =
            std::thread(SubtractCalculatedVariablesCycle, std::ref(matrix), std::ref(result), i, thread_id, std::ref(mtx));
    }
    JoinThreads(threads);
}

void Gauss::SubtractCalculatedVariablesCycle(S21Matrix& matrix, S21Matrix& result, int i, int thread_id, std::mutex& mtx) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices( i + 1, matrix.get_rows(), true);

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        double calculated = matrix(i, j) * result(0, j);
        mtx.lock();
        result(0, i) -= calculated;
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
