#include "ConsoleEngine.h"
namespace s21 {

std::string ConsoleEngine::menu_options_ =
    ("Menu options:\n"
     "\t 1. Solve salesman problem using and algorithm.\n"
     "\t 2. Solve SLAE using the GaussAlgorithm method.\n"
     "\t 3. Multiply matrices using the Winograd algorithm\n"
     "\t 0. Close the application.\n");

ConsoleEngine::~ConsoleEngine() {
    if (!abstract_algorithm_) delete abstract_algorithm_;
}

void ConsoleEngine::start() {
    std::cout << menu_options_;

    while (true) {
        int answer = -1;
        std::string input;
        std::cin >> input;
        if (input.size() == 1 && input.at(0) >= '0' && input.at(0) <= '9') {
            answer = stoi(input);
        }

        S21Matrix matrix1, matrix2;
        switch (answer) {
            case ANT_ALGORITHM:  // 1
                std::cout << "Not implemented" << std::endl;
                continue;
            case GAUSS_ALGORITHM:
                if (!abstract_algorithm_) delete abstract_algorithm_;
                abstract_algorithm_ = new Gauss;
                PrintResults();
                continue;
            case WINOGRAD_ALGORITHM:
                std::cout << "Not implemented" << std::endl;
                continue;
            case EXIT:
                break;
            default:
                std::cout << "Enter number from 0 to 3" << std::endl;
                continue;
        }
        break;
    }
}

void ConsoleEngine::PrintResults() {
    S21Matrix matrix = RequestMatrixFromUser();
    int number_of_repetitions = RequestNumberOfRepetitions();
    std::cout << "Output without using parallelism:" << std::endl;
    PrintMatrix(abstract_algorithm_->SolveWithoutUsingParallelism({matrix}));
    std::cout << "Output using parallelism:" << std::endl;
    PrintMatrix(abstract_algorithm_->SolveUsingParallelism({matrix}));
    std::pair<int, int> result = abstract_algorithm_->MeasureTime({matrix}, number_of_repetitions);
    std::cout << "Seconds spent without using parallelism: " << result.first << std::endl;
    std::cout << "Seconds spent using parallelism: " << result.second << std::endl;
}

S21Matrix ConsoleEngine::RequestMatrixFromUser() {
    std::string filename = RequestFilenameFromUser();
    //    std::string filename = "../TextFiles/GaussMethod3.txt";
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

std::string ConsoleEngine::RequestFilenameFromUser() {
    std::cout << "Enter the text file name: ";
    std::string filename;
    std::cin >> filename;
    return filename;
}

void ConsoleEngine::PrintMatrix(S21Matrix matrix) {
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

int ConsoleEngine::RequestNumberOfRepetitions() {
    std::cout << "Enter the number of repetitions: ";
    int number;
    std::cin >> number;
    if (number < 1) {
        std::cout << "The number must be greater than zero." << std::endl;
        number = RequestNumberOfRepetitions();
    }
    return number;
}
}  // namespace s21