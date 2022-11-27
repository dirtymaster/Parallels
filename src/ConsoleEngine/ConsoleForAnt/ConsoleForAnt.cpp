#include "ConsoleForAnt.h"

namespace s21 {
ConsoleForAnt::ConsoleForAnt() : AbstractConsoleEngine() {
    start_message_ = "This program allows to test performance in "\
        "Ant Colony Algorithm and compare multithreading and single thread "\
        "implementation.\n";
}

void ConsoleForAnt::WrongInputMessage() {
    cout << "Wrong value! Try again\n";
}

int ConsoleForAnt::InputValue(int bottom_border) {
    int val = -1;
    while (val < bottom_border) {
        std::cin >> val;
        if (val < bottom_border) {
            WrongInputMessage();
        }
    }
    return val;
}

void ConsoleForAnt::RequestParamsFromUser() {
    int N, rows;
    cout << "Enter the count of iterations:\n";
    N = InputValue(1);
    cout << "Enter the edge of square matrix:\n";
    rows = InputValue(2);
    S21Matrix matrix(rows, rows);
    std::cout << "Press 0 if you want to fill matrix by yourself "\
                "or 1 if you want to fill matrix with random values\n";
    bool rand;
    std::cin >> rand;
    if (rand) {
        S21Matrix::FillMatrixWithRandValues(&matrix);
    } else {
        cout << "Fill the matrix:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                matrix(i, j) = InputValue(0);
            }
        }
    }
    ant_solver_.SetData(matrix, N);
    cout << "End of initialization\n\n";
}

void ConsoleForAnt::RunAlgorithm() {
    auto start = std::chrono::steady_clock::now();
    ant_solver_.SolveWithoutUsingParallelism();
    duration_without_parallelism_ = std::chrono::steady_clock::now() - start;
    single_thread_result_ = ant_solver_.GetResult();

    start = std::chrono::steady_clock::now();
    ant_solver_.SolveUsingParallelism();
    duration_with_parallelism_ = std::chrono::steady_clock::now() - start;
    multithread_result_ = ant_solver_.GetResult();
}

void ConsoleForAnt::ShowData(double time, TsmResult &path) {
    printf("Time duration: %lf\n", time);
    printf("Path:\n");
    for (auto iter : path.vertices) {
        printf("%d, ", iter);
    }
    printf("\nDistance: %.f\n", path.distance);
}

void ConsoleForAnt::PrintResult() {
    cout << "Single-thread results:\n";
    ShowData(duration_without_parallelism_.count(), single_thread_result_);
    cout << "\nMultithread results\n";
    ShowData(duration_with_parallelism_.count(), multithread_result_);
}
}  // namespace s21
