#include "WinogradAlgorithm.h"

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
    s21::S21Matrix m1(3, 5);
    s21::S21Matrix m2(5, 4);

    Fullfill_matrix(m1);
    Fullfill_matrix(m2);
    Print_matrix(m1);
    Print_matrix(m2);

    s21::WinogradAlgorithm algorithm;
    s21::S21Matrix res = algorithm.SolveWithoutUsingParallelism({m1, m2});
    Print_matrix(res);

    return 0;
}