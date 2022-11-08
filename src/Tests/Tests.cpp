#include <gtest/gtest.h>

#include <string>

#include "../DataStructures/Matrix/Matrix.h"
#include "../Algorithms/AntAlgorithm.h"

TEST(AntAlgorithmTests, Test) {
    s21::S21Matrix matrix1(10, 10);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver.SetData(matrix1, 10);

    TsmResult default_algo = ant_solver.SolveWithoutUsingParallelism();
    TsmResult threading_algo = ant_colver.SolveWithUsingParallelism();
    EXPECT_TRUE(default_algo.distance == threading_algo.distance);
}

TEST(AntAlgorithmTests, Test) {
    s21::S21Matrix matrix1(10, 10);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver.SetData(matrix1, 5);
    TsmResult default_algo = ant_solver.SolveWithoutUsingParallelism();

    s21::S21Matrix matrix2(3, 3);
    s21::S21Matrix::FillMatrixWithRandValues(matrix2);
    ant_solver_.SetData(matrix2, 5);
    TsmResult threading_algo = ant_colver.SolveWithUsingParallelism();
    EXPECT_FALSE(default_algo.distance == threading_algo.distance);
}

TEST(AntAlgorithmTests, Test) {
    s21::S21Matrix matrix1(1, 1);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver.SetData(matrix1, 3);

    TsmResult default_algo = ant_solver.SolveWithoutUsingParallelism();
    TsmResult threading_algo = ant_colver.SolveWithUsingParallelism();
    EXPECT_TRUE(default_algo.distance == threading_algo.distance);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
