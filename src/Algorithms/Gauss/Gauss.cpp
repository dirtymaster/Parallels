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
            Parallelization1(matrix, tmp, i);
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                tmp = matrix(j, i);
                Parallelization2(matrix, tmp, i, j);
            }
        }
        result(0, matrix.get_rows() - 1) = matrix(matrix.get_rows() - 1, matrix.get_rows());
        for (int i = matrix.get_rows() - 2; i >= 0; --i) {
            result(0, i) = matrix(i, matrix.get_rows());
            Parallelization3(matrix, result, i);
        }
    }
    return result;
}

void Gauss::Parallelization1(S21Matrix& matrix, double& tmp, int i) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork1, std::ref(matrix), std::ref(tmp), i, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork1(S21Matrix& matrix, double& tmp, int i, int thread_id) {
    int start_index =
        (int)(matrix.get_rows() - (double)(matrix.get_rows() - (i - 1)) / number_of_threads_ * thread_id);
    int end_index = (i - 1) + (matrix.get_rows() - (i - 1)) / number_of_threads_ *
                                  (number_of_threads_ - 1 - thread_id);

    for (int j = start_index; j > end_index; --j) {
        matrix(i, j) /= tmp;
    }
}

void Gauss::Parallelization2(S21Matrix& matrix, double tmp, int i, int j) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork2, std::ref(matrix), tmp, i, j, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork2(S21Matrix& matrix, double tmp, int i, int j, int thread_id) {
    int start_index =
        (int)(matrix.get_rows() - (double)(matrix.get_rows() - (i - 1)) / number_of_threads_ * thread_id);
    int end_index = (i - 1) + (matrix.get_rows() - (i - 1)) / number_of_threads_ *
                                  (number_of_threads_ - 1 - thread_id);

    for (int k = start_index; k > end_index; --k) {
        matrix(j, k) -= tmp * matrix(i, k);
    }
}

void Gauss::Parallelization3(S21Matrix& matrix, S21Matrix& result, int i) {
    for (int thread_id = 0; thread_id < number_of_threads_; ++thread_id) {
        threads_[thread_id] = new std::thread(DoWork3, std::ref(matrix), std::ref(result), i, thread_id);
    }
    JoinThreadsFromTo();
    DeleteThreadsFromTo();
}

void Gauss::DoWork3(S21Matrix& matrix, S21Matrix& result, int i, int thread_id) {
    int start_index = (i + 1) + (matrix.get_rows() - (i + 1)) / number_of_threads_ *
                                    (number_of_threads_ - 1 - thread_id);
    int end_index =
        (int)(matrix.get_rows() - (double)(matrix.get_rows() - (i + 1)) / number_of_threads_ * thread_id);

    for (int j = start_index; j < end_index; ++j) {
        result(0, i) -= matrix(i, j) * result(0, j);
    }
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
