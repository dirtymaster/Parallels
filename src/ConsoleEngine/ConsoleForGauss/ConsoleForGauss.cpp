#include "ConsoleForGauss.h"

namespace s21 {
ConsoleForGauss::ConsoleForGauss() {
    abstract_algorithm_ = new Gauss;
    start_message_ = "=== Solution of SLAE by Gauss method ===";
}

ConsoleForGauss::~ConsoleForGauss() { delete abstract_algorithm_; }

void ConsoleForGauss::PrintResult() {
    std::cout << "Output without using parallelism:" << std::endl;
    PrintMatrix(result_without_using_parallelism_);
    std::cout << "Output using parallelism:" << std::endl;
    PrintMatrix(result_using_parallelism_);
    std::cout << "Seconds spent without using parallelism: " << times_.first << std::endl;
    std::cout << "Seconds spent using parallelism: " << times_.second << std::endl;
}

void ConsoleForGauss::RunAlgorithm() {
    result_without_using_parallelism_ = abstract_algorithm_->SolveWithoutUsingParallelism({matrix_});
    result_using_parallelism_ = abstract_algorithm_->SolveUsingParallelism({matrix_});
    times_ = abstract_algorithm_->MeasureTime({matrix_}, number_of_repetitions_);
}

void ConsoleForGauss::RequestParamsFromUser() {
    matrix_ = RequestMatrixFromUser();
    number_of_repetitions_ = RequestNumberOfRepetitions();
}

S21Matrix ConsoleForGauss::RequestMatrixFromUser() {
    //    std::string filename = RequestFilenameFromUser();
    std::string filename = "../TextFiles/GaussMethod3.txt";
    S21Matrix result;
    std::fstream fs;
    fs.open(filename, std::fstream::in);
    if (!fs.is_open()) return result;
    int rows, cols;
    fs >> rows;
    fs >> cols;
    result.set_rows(rows);
    result.set_columns(cols);
    if (rows <= 0 || cols <= 0) return result;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value;
            if (!(fs >> value)) return S21Matrix();
            result(i, j) = value;
        }
    }
    return result;
}

string ConsoleForGauss::RequestFilenameFromUser() {
    std::cout << "Enter the text file name: ";
    std::string filename;
    std::cin >> filename;
    return filename;
}

int ConsoleForGauss::RequestNumberOfRepetitions() {
    std::cout << "Enter the number of repetitions: ";
    int number;
    std::cin >> number;
    if (number < 1) {
        std::cout << "The number must be greater than zero." << std::endl;
        number = RequestNumberOfRepetitions();
    }
    return number;
}

void ConsoleForGauss::PrintMatrix(S21Matrix matrix) {
    if (matrix.get_rows() == 0 || matrix.get_cols() == 0) {
        std::cout << "The matrix is empty" << std::endl;
        return;
    }
    for (int i = 0; i < matrix.get_rows(); ++i) {
        for (int j = 0; j < matrix.get_cols(); ++j) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }
}
}  // namespace s21
