
#ifndef PARALLELS_WINOGRADALGORITHM_H
#define PARALLELS_WINOGRADALGORITHM_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "../AbstractAlgorithm.h"

namespace s21 {

class WinogradAlgorithm {
public:

    S21Matrix SolveWithoutParallelism(S21Matrix *M1, S21Matrix *M2);
    S21Matrix SolveWithPipelineParallelism(S21Matrix *M1, S21Matrix *M2);
    S21Matrix SolveWithClassicParallelism(S21Matrix *M1, S21Matrix *M2, int threads);

private:
    double *row_factors_;
    double *column_factors_;
    S21Matrix *M1_;
    S21Matrix *M2_;
    S21Matrix res_;
    int len_;

    bool CheckIfMatricesCorrect(S21Matrix *M1, S21Matrix *M2);
    void SetupParameters(S21Matrix *M1, S21Matrix *M2);
    S21Matrix HandleCornerCase(S21Matrix *M1, S21Matrix *M2);

    void CalculateRowFactors(int start_ind, int end_ind);
    void CalculateColumnFactors(int start_ind, int end_ind);
    void CalculateResultMatrixValues(int start_ind, int end_ind);
    void PrepareColumnAndRowFactors(int start_ind1, int end_ind1,
                                    int start_ind2, int end_ind2);


    // PIPELINE REALISATION //
    std::mutex matrix_mtx_;
    std::mutex row_factors_mtx_;
    std::mutex column_factors_mtx_;
    std::condition_variable row_factors_cv_;
    std::condition_variable column_factors_cv_;
    std::condition_variable matrix_cv_;

    bool row_factors_ready_;
    bool column_factors_ready_;
    bool stage_three_ready_;

    void StageOne();
    void StageTwo();
    void StageThree();
    void StageFour();
};

}

#endif //PARALLELS_WINOGRADALGORITHM_H
