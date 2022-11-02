#include "Gauss.h"
namespace s21 {
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

        std::mutex m;
        for (int i = 0; i < matrix.get_rows(); ++i) {
            tmp = matrix(i, i);
            int num_threads = matrix.get_rows();
            std::vector<std::thread> threads(num_threads);
            auto thread_func = [&matrix, &tmp, &num_threads, &i](int thread_id) {
                int start_index = matrix.get_rows() - (double)(matrix.get_rows() - (i - 1)) / num_threads * thread_id;
                int end_index =
                    (i - 1) + (matrix.get_rows() - (i - 1)) / num_threads * (num_threads - 1 - thread_id);

                for (int j = start_index; j > end_index; --j) {
                    matrix(i, j) /= tmp;
                }
            };
            for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
                threads[thread_id] = std::thread(thread_func, thread_id);
            }
            for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
                threads[thread_id].join();
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
}  // namespace s21
