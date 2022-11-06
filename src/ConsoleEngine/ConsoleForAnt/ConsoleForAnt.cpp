#include "ConsoleForAnt.h"

namespace s21 {
    ConsoleForAnt::ConsoleForAnt() : AbstractConsoleEngine() {}

    void ConsoleForAnt::RequestParamsFromUser() {
        int N;
        cout << "Enter the count of iterations:\n";
        cin >> N;
        int rows;
        cout << "Enter the size of square matrix:";
        cin >> rows;
        S21Matrix matrix(rows, rows);
        cout << "Fill the matrix:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                cin >> matrix(i, j);
            }
        }
        ant_solver_ = AntAlgorithm(matrix, N);
    }

    void ConsoleForAnt::RunAlgorithm() {
        auto start = std::chrono::high_resolution_clock::now();
        ant_solver_.SolveWithoutUsingParallelism();
        auto duration = std::chrono::high_resolution_clock::now() - start;
        cout << "Default algorithm takes " << duration.count() << " microseconds\n";

//        start = std::chrono::high_resolution_clock::now();
//        ant_solver_.SolveUsingParallelism();
//        duration = std::chrono::high_resolution_clock::now() - start;
//        cout << "Algorithm with parallelism takes " << duration.count() << " microseconds\n";
    }

    void ConsoleForAnt::PrintResult() {
        ;
    }
}
