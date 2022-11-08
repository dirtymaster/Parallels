#include "ConsoleForGauss.h"

namespace s21 {
ConsoleForGauss::ConsoleForGauss() {
    abstract_algorithm_ = new GaussAlgorithm;
    filename_ = "";
    start_message_ = "=== Solution of SLAE by GaussAlgorithm method ===";
}

ConsoleForGauss::~ConsoleForGauss() { delete abstract_algorithm_; }

void ConsoleForGauss::PrintResult() {
    std::cout << "Output without using parallelism:" << std::endl;
    PrintMatrix(results_.first);
    std::cout << "Output using parallelism:" << std::endl;
    PrintMatrix(results_.second);
    if (!result_without_using_parallelism_.is_empty() && !result_using_parallelism_.is_empty()) {
        std::cout << "Seconds spent without using parallelism: " << times_.first << std::endl;
        std::cout << "Seconds spent using parallelism: " << times_.second << std::endl;
    }
}

void ConsoleForGauss::RunAlgorithm() {
    times_ = abstract_algorithm_->MeasureTime({matrix_}, results_, number_of_repetitions_);
    result_without_using_parallelism_ = results_.first;
    result_using_parallelism_ = results_.second;
    number_of_repetitions_ = -1;
    filename_ = "";
}

void ConsoleForGauss::RequestParamsFromUser() {
    matrix_ = RequestMatrixFromUser();
    number_of_repetitions_ = RequestNumberOfRepetitions();
}

S21Matrix ConsoleForGauss::RequestMatrixFromUser() {
    if (filename_ == "") {
        RequestFilenameFromUser();
    }
    S21Matrix result;
    std::fstream fs;
    fs.open(filename_, std::fstream::in);
    if (!fs.is_open()) {
        std::cout << "Invalid file name." << std::endl;
        filename_ = "";
        return RequestMatrixFromUser();
    }
    int rows, cols;
    fs >> rows;
    fs >> cols;
    result = S21Matrix(rows, cols);
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

void ConsoleForGauss::RequestFilenameFromUser() {
    std::cout << "Enter the text file name: ";
    std::cin >> filename_;
}

int ConsoleForGauss::RequestNumberOfRepetitions() {
    int number;
    if (number_of_repetitions_ == -1) {
        std::cout << "Enter the number of repetitions: ";

        std::cin >> number;
        if (number < 1) {
            std::cout << "The number must be greater than zero." << std::endl;
            number = RequestNumberOfRepetitions();
        }
    } else {
        return number_of_repetitions_;
    }
    return number;
}

void ConsoleForGauss::PrintMatrix(S21Matrix matrix) {
    if (matrix.get_rows() == 0 || matrix.get_cols() == 0) {
        std::cout << "The input matrix has incorrect parameters" << std::endl;
        return;
    }
    for (int i = 0; i < matrix.get_rows(); ++i) {
        for (int j = 0; j < matrix.get_cols(); ++j) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

void ConsoleForGauss::GenerateRandomMatrix() {
    std::string filename = "/Users/rafael/Projects/Parallels/src/TextFiles/GaussMethod4.txt";
    std::fstream fs(filename, std::fstream::out);
    int rows = 9999;
    int cols = 10000;
    fs << rows << " " << cols << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fs << rand() % 10 << " ";
        }
        fs << std::endl;
    }
}
}  // namespace s21
