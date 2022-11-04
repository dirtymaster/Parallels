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
    s21::S21Matrix m1(1000, 1000);
    s21::S21Matrix m2(1000, 1000);

    Fullfill_matrix(m1);
    Fullfill_matrix(m2);
//    Print_matrix(m1);
//    Print_matrix(m2);

    s21::WinogradAlgorithm algorithm;
    auto start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix res = algorithm.SolveWithoutUsingParallelism({m1, m2});
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nWinograd %lf:\n", duration.count());
    //Print_matrix(res);

    start = std::chrono::high_resolution_clock::now();
    s21::S21Matrix m3 = m1 * m2;
    duration = std::chrono::high_resolution_clock::now() - start;
    printf("\nBasic %lf:\n", duration.count());
    //Print_matrix(m3 );


    return 0;
}