
#ifndef PARALLELS_WINOGRADALGORITHM_H
#define PARALLELS_WINOGRADALGORITHM_H

#include "../AbstractAlgorithm.h"

namespace s21 {

class WinogradAlgorithm : public AbstractAlgorithm {
public:
    S21Matrix SolveWithoutUsingParallelism(std::vector <S21Matrix> matrices);
    S21Matrix SolveUsingParallelism(std::vector <S21Matrix> matrices);

private:

};

}

#endif //PARALLELS_WINOGRADALGORITHM_H
