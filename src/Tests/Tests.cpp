#include <gtest/gtest.h>

#include <string>

#include "../Algorithms/AntColonyAlgorithm/AntAlgorithm.h"
#include "../Algorithms/GaussAlgorithm/GaussAlgorithm.h"
#include "../Algorithms/WinogradAlgorithm/WinogradAlgorithm.h"
#include "../ConsoleEngine/ConsoleForGauss/ConsoleForTestingGauss/ConsoleForTestingGauss.h"
#include "../DataStructures/Matrix/Matrix.h"

TEST(AntAlgorithmTests, Test1) {
    s21::S21Matrix matrix1(10, 10);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver;
    ant_solver.SetData(matrix1, 10);

    ant_solver.SolveWithoutUsingParallelism();
    s21::TsmResult default_algo = ant_solver.GetResult();
    ant_solver.SolveUsingParallelism();
    s21::TsmResult threading_algo = ant_solver.GetResult();
    EXPECT_TRUE(default_algo.distance == threading_algo.distance);
}

TEST(AntAlgorithmTests, Test2) {
    s21::S21Matrix matrix1(10, 10);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver;
    ant_solver.SetData(matrix1, 5);
    ant_solver.SolveWithoutUsingParallelism();
    s21::TsmResult default_algo = ant_solver.GetResult();

    s21::S21Matrix matrix2(3, 3);
    s21::S21Matrix::FillMatrixWithRandValues(matrix2);
    ant_solver.SetData(matrix2, 5);
    ant_solver.SolveUsingParallelism();
    s21::TsmResult threading_algo = ant_solver.GetResult();
    EXPECT_FALSE(default_algo.distance == threading_algo.distance);
}

TEST(AntAlgorithmTests, Test3) {
    s21::S21Matrix matrix1(1, 1);
    s21::S21Matrix::FillMatrixWithRandValues(matrix1);
    s21::AntAlgorithm ant_solver;
    ant_solver.SetData(matrix1, 3);

    ant_solver.SolveWithoutUsingParallelism();
    s21::TsmResult default_algo = ant_solver.GetResult();
    ant_solver.SolveUsingParallelism();
    s21::TsmResult threading_algo = ant_solver.GetResult();
    EXPECT_TRUE(default_algo.distance == threading_algo.distance);
}

TEST(WinogradAlgoTests, EvenMatrices) {
    s21::S21Matrix m1(100, 100);
    s21::S21Matrix m2(100, 100);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 6);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, OddMatrices) {
    s21::S21Matrix m1(123, 123);
    s21::S21Matrix m2(123, 123);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 2);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, DifferentCombination1) {
    s21::S21Matrix m1(42, 111);
    s21::S21Matrix m2(111, 21);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 4);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, DifferentCombination2) {
    s21::S21Matrix m1(33, 1);
    s21::S21Matrix m2(1, 66);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 6);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, DifferentCombination3) {
    s21::S21Matrix m1(1, 21);
    s21::S21Matrix m2(21, 3);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 6);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, DifferentCombination4) {
    s21::S21Matrix m1(4, 42);
    s21::S21Matrix m2(42, 1);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 4);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, DifferentCombination5) {
    s21::S21Matrix m1(2, 2);
    s21::S21Matrix m2(2, 4);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 4);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, LittleMatrix) {
    s21::S21Matrix m1(1, 1);
    s21::S21Matrix m2(1, 1);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = m1 * m2;
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 6);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(WinogradAlgoTests, WrongDimensions) {
    s21::S21Matrix m1(42, 111);
    s21::S21Matrix m2(222, 21);
    s21::S21Matrix::FillMatrixWithRandValues(m1);
    s21::S21Matrix::FillMatrixWithRandValues(m2);
    s21::WinogradAlgorithm algorithm;

    s21::S21Matrix expected = s21::S21Matrix();
    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2, 4);

    EXPECT_TRUE(res1 == expected);
    EXPECT_TRUE(res2 == expected);
    EXPECT_TRUE(res3 == expected);
}

TEST(GaussAlgoTests, Rows3Cols4) {
    s21::S21Matrix expected(1, 3);
    expected(0, 0) = 1;
    expected(0, 1) = 2;
    expected(0, 2) = 3;
    s21::ConsoleForTestingGauss console;

    std::string filename = "TextFiles/GaussMethod1.txt";
    console.SetFileName(filename);
    console.SetNumberOfRepetitions(1);
    console.RequestParamsFromUserForTest();
    console.RunAlgorithmForTest();
    EXPECT_TRUE(console.GetResultWithoutUsingParallelism() == expected);
    EXPECT_TRUE(console.GetResultUsingParallelism() == expected);
}

TEST(GaussAlgoTests, Rows4Cols5) {
    s21::S21Matrix expected(1, 4);
    expected(0, 0) = -3;
    expected(0, 1) = -1;
    expected(0, 2) = 2;
    expected(0, 3) = 7;
    s21::ConsoleForTestingGauss console;

    std::string filename = "TextFiles/GaussMethod2.txt";
    console.SetFileName(filename);
    console.SetNumberOfRepetitions(1);
    console.RequestParamsFromUserForTest();
    console.RunAlgorithmForTest();
    EXPECT_TRUE(console.GetResultWithoutUsingParallelism() == expected);
    EXPECT_TRUE(console.GetResultUsingParallelism() == expected);
}

TEST(GaussAlgoTests, Rows99Cols100) {
    s21::ConsoleForTestingGauss console;

    std::string filename = "TextFiles/GaussMethod3.txt";
    console.SetFileName(filename);
    console.SetNumberOfRepetitions(1);
    console.RequestParamsFromUserForTest();
    console.RunAlgorithmForTest();
    EXPECT_TRUE(console.GetResultWithoutUsingParallelism() == console.GetResultUsingParallelism());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    //    if (argc && argv) {;}
    //    s21::S21Matrix m1(6, 11);
    //    s21::S21Matrix m2(11, 6);
    //    s21::S21Matrix::FillMatrixWithRandValues(m1);
    //    s21::S21Matrix::FillMatrixWithRandValues(m2);
    //    s21::WinogradAlgorithm algorithm;
    //
    //    s21::S21Matrix expected = m1 * m2;
    //    s21::S21Matrix res1 = algorithm.SolveWithoutParallelism(&m1, &m2);
    //    s21::S21Matrix res2 = algorithm.SolveWithPipelineParallelism(&m1, &m2);
    //    s21::S21Matrix res3 = algorithm.SolveWithClassicParallelism(&m1, &m2);
    //
    //    s21::S21Matrix::Print_matrix(res1);
    //    s21::S21Matrix::Print_matrix(res2);
    //    s21::S21Matrix::Print_matrix(res3);
    //    s21::S21Matrix::Print_matrix(expected);

    return 0;
}
