
#ifndef PARALLELS_CONSOLEFORWINOGRAD_H
#define PARALLELS_CONSOLEFORWINOGRAD_H

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>

#include "../../Algorithms/WinogradAlgorithm/WinogradAlgorithm.h"
#include "../../DataStructures/Matrix/Matrix.h"
#include "../AbstractConsoleEngine.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::sscanf;
using std::string;

namespace s21 {

class ConsoleForWinograd : public AbstractConsoleEngine {
public:
    ConsoleForWinograd();

private:
    S21Matrix *M1_ = nullptr;
    S21Matrix *M2_ = nullptr;
    int rows_;
    int cols_;

    WinogradAlgorithm winograd_algorithm_;
    int nmb_of_repeats_;
    int nmb_of_threads_;

    bool need_to_print_values_;

    std::chrono::duration<double> duration_without_parallelism_;
    std::chrono::duration<double> duration_with_pipeline_parallelism_;
    std::chrono::duration<double> duration_with_classic_parallelism_;

    void RequestParamsFromUser();
    void RunAlgorithm();
    void PrintResult();

    bool AskUserAboutPrintingValues();
    bool GetMatrixInput(S21Matrix **mat);
    int RequestNmbFromUser(string message);
    S21Matrix *ParseFileWithMatrix(fstream file);
};

}  // namespace s21

#endif  // PARALLELS_CONSOLEFORWINOGRAD_H
