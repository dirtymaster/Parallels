#include "Gauss.h"
namespace s21 {
int Gauss::levels_ = 2;
int Gauss::threads_per_level_ = 2;
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

        std::vector<std::thread*> threads(levels_ * threads_per_level_);
        std::mutex m;

        for (int i = 0; i < matrix.get_rows(); ++i) {
            tmp = matrix(i, i);
            FirstLevelParallelization(matrix, tmp, i, threads);
            //            SecondLevelParallelization(matrix, tmp, i, threads);
            for (int j = i + 1; j < matrix.get_rows(); ++j) {
                tmp = matrix(j, i);
                FirstLevelParallelization2(matrix, tmp, i, j, threads);
            }
        }
        result(0, matrix.get_rows() - 1) = matrix(matrix.get_rows() - 1, matrix.get_rows());
        //        SecondLevelParallelization2(matrix, result, tmp, threads);
        for (int i = matrix.get_rows() - 2; i >= 0; --i) {
            result(0, i) = matrix(i, matrix.get_rows());
            FirstLevelParallelization3(matrix, result, tmp, i, threads);
        }
    }
    return result;
}

void Gauss::DeleteThreadsFromTo(std::vector<std::thread*>& threads, int level) {
    for (int i = (level - 1) * threads_per_level_; i < level * threads_per_level_; ++i) {
        delete threads[i];
    }
}

void Gauss::JoinThreadsFromTo(std::vector<std::thread*>& threads, int level) {
    for (int i = (level - 1) * threads_per_level_; i < level * threads_per_level_; ++i) {
        threads[i]->join();
    }
}

void Gauss::FirstLevelParallelization(S21Matrix& matrix, double& tmp, int i,
                                      std::vector<std::thread*> threads) {
    int level = 1;
    auto thread_func = [&matrix, &tmp, &threads, i](int thread_id) {
        int start_index =
            matrix.get_rows() - (double)(matrix.get_rows() - (i - 1)) / threads_per_level_ * thread_id;
        int end_index = (i - 1) + (matrix.get_rows() - (i - 1)) / threads_per_level_ *
                                      (threads_per_level_ - 1 - thread_id);

        for (int j = start_index; j > end_index; --j) {
            matrix(i, j) /= tmp;
        }
    };
    for (int thread_id = 0; thread_id < threads_per_level_; ++thread_id) {
        threads[thread_id + (level - 1) * threads_per_level_] = new std::thread(thread_func, thread_id);
    }
    JoinThreadsFromTo(threads, level);
    DeleteThreadsFromTo(threads, level);
}

void Gauss::FirstLevelParallelization2(S21Matrix& matrix, double tmp, int i, int j,
                                       std::vector<std::thread*> threads) {
    int level = 1;
    auto thread_func = [&matrix, tmp, &threads, i, j](int thread_id) {
        int start_index =
            matrix.get_rows() - (double)(matrix.get_rows() - (i - 1)) / threads_per_level_ * thread_id;
        int end_index = (i - 1) + (matrix.get_rows() - (i - 1)) / threads_per_level_ *
                                      (threads_per_level_ - 1 - thread_id);

        for (int k = start_index; k > end_index; --k) {
            matrix(j, k) -= tmp * matrix(i, k);
        }
    };
    for (int thread_id = 0; thread_id < threads_per_level_; ++thread_id) {
        threads[thread_id + (level - 1) * threads_per_level_] = new std::thread(thread_func, thread_id);
    }
    JoinThreadsFromTo(threads, level);
    DeleteThreadsFromTo(threads, level);
}

//void Gauss::SecondLevelParallelization(S21Matrix& matrix, double& tmp, int i,
//                                       std::vector<std::thread*> threads) {
//    int level = 2;
//    auto thread_func = [&matrix, &tmp, &threads, i](int thread_id) {
//        int start_index = (i + 1) + (matrix.get_rows() - (i + 1)) / threads_per_level_ *
//                                        (threads_per_level_ - 1 - thread_id);
//        int end_index =
//            matrix.get_rows() - (double)(matrix.get_rows() - (i + 1)) / threads_per_level_ * thread_id;
//
//        for (int j = start_index; j < end_index; ++j) {
//            tmp = matrix(j, i);
//            Gauss::FirstLevelParallelization2(matrix, tmp, i, j, threads);
//        }
//    };
//    for (int thread_id = 0; thread_id < threads_per_level_; ++thread_id) {
//        threads[thread_id + (level - 1) * threads_per_level_] = new std::thread(thread_func, thread_id);
//    }
//    JoinThreadsFromTo(threads, level);
//    DeleteThreadsFromTo(threads, level);
//}

void Gauss::FirstLevelParallelization3(S21Matrix& matrix, S21Matrix& result, double tmp, int i,
                                       std::vector<std::thread*> threads) {
    int level = 1;
    auto thread_func = [&matrix, &result, tmp, i, &threads](int thread_id) {
        int start_index = (i + 1) + (matrix.get_rows() - (i + 1)) / threads_per_level_ *
                                        (threads_per_level_ - 1 - thread_id);
        int end_index =
            matrix.get_rows() - (double)(matrix.get_rows() - (i + 1)) / threads_per_level_ * thread_id;

        for (int j = start_index; j < end_index; ++j) {
            result(0, i) -= matrix(i, j) * result(0, j);
        }
    };
    for (int thread_id = 0; thread_id < threads_per_level_; ++thread_id) {
        threads[thread_id + (level - 1) * threads_per_level_] = new std::thread(thread_func, thread_id);
    }
    JoinThreadsFromTo(threads, level);
    DeleteThreadsFromTo(threads, level);
}

//void Gauss::SecondLevelParallelization2(S21Matrix& matrix, S21Matrix& result, double tmp,
//                                        std::vector<std::thread*> threads) {
//    int level = 2;
//    auto thread_func = [&matrix, &result, &tmp, &threads](int thread_id) {
//        int start_index = (matrix.get_rows() - 2) -
//                          (double)((matrix.get_rows() - 2) - (-1)) / threads_per_level_ * thread_id;
//        int end_index =
//            -1 + ((matrix.get_rows() - 2) - (-1)) / threads_per_level_ * (threads_per_level_ - 1 - thread_id);
//
//        for (int i = start_index; i > end_index; --i) {
//            result(0, i) = matrix(i, matrix.get_rows());
//            FirstLevelParallelization3(matrix, result, tmp, i, threads);
//        }
//    };
//    for (int thread_id = 0; thread_id < threads_per_level_; ++thread_id) {
//        threads[thread_id + (level - 1) * threads_per_level_] = new std::thread(thread_func, thread_id);
//    }
//    JoinThreadsFromTo(threads, level);
//    DeleteThreadsFromTo(threads, level);
//}
}  // namespace s21
