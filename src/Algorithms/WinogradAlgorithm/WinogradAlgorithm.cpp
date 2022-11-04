
#include "WinogradAlgorithm.h"

namespace s21 {

S21Matrix WinogradAlgorithm::SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
        exit(1);
    }

    if (matrices[0].get_cols() != matrices[1].get_rows()) {
        printf("get cols != get rows\n");
        exit(2);
    }

//    S21Matrix result(matrices[0].get_rows(), matrices[1].get_cols());
    S21Matrix result = matrices[0] * matrices[1];
    return result;
}

S21Matrix WinogradAlgorithm::SolveUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
    }

    if (matrices[0].get_cols() != matrices[1].get_rows()) {

    }

    S21Matrix result;

    return result;
}



}  // namespace s21