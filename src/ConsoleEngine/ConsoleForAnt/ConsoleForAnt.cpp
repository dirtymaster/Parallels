#include "ConsoleForAnt.h"

namespace s21 {
ConsoleForAnt::ConsoleForAnt() : AbstractConsoleEngine() { correct_data = true; }

void ConsoleForAnt::RequestParamsFromUser() {
    int N;
    std::cout << "Enter the count of iterations:\n";
    std::cin >> N;
    int rows;
    std::cout << "Enter the edge of square matrix:\n";
    std::cin >> rows;
    if (N < 0 || rows < 0) {
        correct_data = false;
        cout << "Wrong input data!\n";
        return;
    }
    S21Matrix matrix(rows, rows);
    // S21Matrix::FillMatrixWithRandValues(matrix);
    cout << "Fill the matrix:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            std::cin >> matrix(i, j);
        }
    }
    ant_solver_.SetData(matrix, N);
    std::cout << "End of initialization\n\n";
}

void ConsoleForAnt::RunAlgorithm() {
    if (!correct_data) {
        return;
    }
    auto start = std::chrono::steady_clock::now();
    ant_solver_.SolveWithoutUsingParallelism();
    auto duration = std::chrono::steady_clock::now() - start;
    cout << "\nDefault algorithm takes " << duration.count() << " microseconds\n\n";

    std::cout << ant_solver_.GetResult().distance << "\n";

    start = std::chrono::steady_clock::now();
    ant_solver_.SolveUsingParallelism();
    duration = std::chrono::steady_clock::now() - start;
    cout << "Algorithm with parallelism takes " << duration.count() << " microseconds\n";

    std::cout << ant_solver_.GetResult().distance << "\n";
}

void ConsoleForAnt::PrintResult() { ; }
}  // namespace s21
