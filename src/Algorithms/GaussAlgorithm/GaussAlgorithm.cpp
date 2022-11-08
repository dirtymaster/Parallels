#include "GaussAlgorithm.h"
namespace s21 {
int GaussAlgorithm::threads_in_level_;

std::pair<double, double> GaussAlgorithm::MeasureTime(S21Matrix matrix,
                                                         std::pair<S21Matrix, S21Matrix>& results,
                                                         int number_of_repetitions) {
    std::pair<double, double> times;
    auto start_time = std::chrono::high_resolution_clock::now();
    results.first = SolveWithoutUsingParallelism(matrix);  // записываем результат работы
    for (int i = 1; i < number_of_repetitions; ++i) {
        SolveWithoutUsingParallelism(matrix);
    }
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start_time;
    times.first = duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    results.second = SolveUsingParallelism(matrix);  // записываем результат работы
    for (int i = 1; i < number_of_repetitions; ++i) {
        SolveUsingParallelism(matrix);
    }
    duration = std::chrono::high_resolution_clock::now() - start_time;
    times.second = duration.count();
    return times;
}

S21Matrix GaussAlgorithm::SolveWithoutUsingParallelism(S21Matrix matrix) {
    S21Matrix result;
    if (matrix.get_rows() >= 2 && matrix.get_cols() == matrix.get_rows() + 1) {
        double tmp;
        result.set_rows(1);
        result.set_columns(matrix.get_rows());
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

S21Matrix GaussAlgorithm::SolveUsingParallelism(S21Matrix matrix) {
    S21Matrix result;
    if (matrix.get_rows() >= 2 && matrix.get_cols() == matrix.get_rows() + 1) {
        threads_in_level_ = std::thread::hardware_concurrency() - 1;
        threads_in_level_ = matrix.get_cols() < threads_in_level_ ? matrix.get_cols() : threads_in_level_;
        result.set_rows(1);
        result.set_columns(matrix.get_rows());

        int rows = matrix.get_rows();
        for (int i = 0; i < rows; ++i) {
            DivideEquation(matrix, matrix(i, i), i);
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

void GaussAlgorithm::DivideEquation(S21Matrix& matrix, double tmp, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(DivideEquationCycle, std::ref(matrix), tmp, i, thread_id);
    }
    JoinThreads(threads);
}

void GaussAlgorithm::DivideEquationCycle(S21Matrix& matrix, double tmp, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows(), i - 1, false);

    for (int j = start_and_end_indices.first[thread_id]; j > start_and_end_indices.second[thread_id]; --j) {
        matrix(i, j) /= tmp;
    }
}

void GaussAlgorithm::SubtractElementsInMatrix(S21Matrix& matrix, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(SubtractElementsInMatrixCycle, std::ref(matrix), i, thread_id);
    }
    JoinThreads(threads);
}

void GaussAlgorithm::SubtractElementsInMatrixCycle(S21Matrix& matrix, int i, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(i + 1, matrix.get_rows(), true);

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        double tmp = matrix(j, i);
        for (int k = matrix.get_rows(); k >= i; --k) {
            matrix(j, k) -= tmp * matrix(i, k);
        }
    }
}

void GaussAlgorithm::EquateResultsToRightValues(s21::S21Matrix& matrix, s21::S21Matrix& result) {
    std::vector<std::thread> threads(threads_in_level_);
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] =
            std::thread(EquateResultsToRightValuesCycle, std::ref(matrix), std::ref(result), thread_id);
    }
    JoinThreads(threads);
}

void GaussAlgorithm::EquateResultsToRightValuesCycle(S21Matrix& matrix, S21Matrix& result, int thread_id) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(matrix.get_rows() - 2, -1, false);

    for (int i = start_and_end_indices.first[thread_id]; i > start_and_end_indices.second[thread_id]; --i) {
        result(0, i) = matrix(i, matrix.get_rows());
    }
}

void GaussAlgorithm::SubtractCalculatedVariables(s21::S21Matrix& matrix, s21::S21Matrix& result, int i) {
    std::vector<std::thread> threads(threads_in_level_);
    std::mutex mtx;
    for (int thread_id = 0; thread_id < threads_in_level_; ++thread_id) {
        threads[thread_id] = std::thread(SubtractCalculatedVariablesCycle, std::ref(matrix), std::ref(result),
                                         i, thread_id, std::ref(mtx));
    }
    JoinThreads(threads);
}

void GaussAlgorithm::SubtractCalculatedVariablesCycle(S21Matrix& matrix, S21Matrix& result, int i,
                                                      int thread_id, std::mutex& mtx) {
    std::pair<std::vector<int>, std::vector<int>> start_and_end_indices =
        InitializeStartAndEndIndices(i + 1, matrix.get_rows(), true);

    for (int j = start_and_end_indices.first[thread_id]; j < start_and_end_indices.second[thread_id]; ++j) {
        double calculated = matrix(i, j) * result(0, j);
        mtx.lock();
        result(0, i) -= calculated;
        mtx.unlock();
    }
}

std::pair<std::vector<int>, std::vector<int>> GaussAlgorithm::InitializeStartAndEndIndices(
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

void GaussAlgorithm::JoinThreads(std::vector<std::thread>& threads) {
    for (int i = 0; i < threads_in_level_; ++i) {
        threads[i].join();
    }
}
}  // namespace s21
