#include <gtest/gtest.h>
#include "../DataStructures/Matrix/Matrix.h"
#include "../Algorithms/WinogradAlgorithm/WinogradAlgorithm.h"

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