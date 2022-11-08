
#ifndef PARALLELS_CONSOLEFORWINOGRAD_H
#define PARALLELS_CONSOLEFORWINOGRAD_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include "../AbstractConsoleEngine.h"
#include "../../DataStructures/Matrix/Matrix.h"
#include "../../Algorithms/WinogradAlgorithm/WinogradAlgorithm.h"

using std::cout;
using std::cin;
using std::endl;
using std::sscanf;
using std::fstream;
using std::string;

namespace s21 {

class ConsoleForWinograd : public AbstractConsoleEngine {

public:
    ConsoleForWinograd();

private:
    S21Matrix *M1_;
    S21Matrix *M2_;
    int rows_;
    int cols_;

    WinogradAlgorithm winograd_algorithm_;
    int nmb_of_repeats_;
    int nmb_of_threads_;

    std::chrono::duration<double> duration_without_parallelism_;
    std::chrono::duration<double> duration_with_pipeline_parallelism_;
    std::chrono::duration<double> duration_with_classic_parallelism_;

    void RequestParamsFromUser();
    void RunAlgorithm();
    void PrintResult();

    bool GetMatrixInput(S21Matrix *mat);
    void ParseFileWithMatrix();
    int RequestNmbFromUser(string message);

};

}  // namespace s21

#endif //PARALLELS_CONSOLEFORWINOGRAD_H
