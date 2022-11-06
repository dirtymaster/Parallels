#include "WinogradAlgorithm.h"
#include <chrono>

void Fullfill_matrix(s21::S21Matrix &m1) {
    double n = 1;
    for (int i = 0; i < m1.get_rows(); i++) {
        for (int j = 0; j < m1.get_cols(); j++) {
            m1(i, j) = n++;
        }
    }
}

void Print_matrix(s21::S21Matrix m1) {
    for (int i = 0; i < m1.get_rows(); i++) {
        for (int j = 0; j < m1.get_cols(); j++) {
            printf("%3.1lf ", m1(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
//    s21::S21Matrix m1(499, 499);
//    s21::S21Matrix m2(499, 499);

    s21::S21Matrix m1(1001, 1001);
    s21::S21Matrix m2(1001, 1001);

    Fullfill_matrix(m1);
    Fullfill_matrix(m2);

    s21::WinogradAlgorithm algorithm;
    auto start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nWinograd without parallelism = %lf\n", duration.count());

    start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nWinograd with pipeline parallelism = %lf:\n", duration.count());

    start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2);
    duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nWinograd with classic parallelism = %lf:\n", duration.count());

    start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix res4 = m1 * m2;
    duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nBasic mult algo = %lf:\n", duration.count());

    if ((res1 == res2) && (res2 == res3) && (res3 == res4)) {
        printf("Cool\n");
    }
//    s21::S21Matrix m1(5, 4);
//    s21::S21Matrix m2(4, 7);
//
//    Fullfill_matrix(m1);
//    Fullfill_matrix(m2);
//
//    s21::WinogradAlgorithm algorithm;
//    s21::S21Matrix win = algorithm.SolveWithoutUsingParallelism({m1, m2});
//    printf("\nWinograd:\n");
//    Print_matrix(win);
//
//    s21::S21Matrix basic = m1 * m2;
//    printf("\nBasic:\n");
//    Print_matrix(basic);

    return 0;
}