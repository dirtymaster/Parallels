#include "ConsoleEngine.h"
namespace s21 {
ConsoleEngine::~ConsoleEngine() {
    if (abstract_algorithm_) delete abstract_algorithm_;
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
                if (abstract_algorithm_) delete abstract_algorithm_;
                abstract_algorithm_ = new Gauss;
                PrintMatrix(abstract_algorithm_->SolveWithoutUsingParallelism({RequestMatrixFromUser()}));
                continue;
            case WINOGRAD_ALGORITHM:
                std::cout << "Not implemented" << std::endl;
                continue;
            case EXIT:
                break;
        }
        break;
    }
}

S21Matrix ConsoleEngine::RequestMatrixFromUser() {
    std::string filename = RequestFilenameFromUser();
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
            int value;
            if (!(fs >> value)) return S21Matrix();
            result(i, j) = value;
        }
    }
    return result;
}

const std::string ConsoleEngine::RequestFilenameFromUser() {
    std::cout << "Enter the text file name:";
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
}  // namespace s21