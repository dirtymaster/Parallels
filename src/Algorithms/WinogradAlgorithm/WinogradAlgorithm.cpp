
#include "WinogradAlgorithm.h"

namespace s21 {

S21Matrix WinogradAlgorithm::SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices) {
    if (matrices.size() != 2) {
        printf("Need to pass 2 matrices\n");
    }

    int size = matrices[0].get_rows();
    if (matrices[0].get_cols() != size || matrices[1].get_rows() != size || matrices[1].get_cols() != size) {

    }





    S21Matrix result;

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