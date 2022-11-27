
#include "ConsoleForWinograd.h"

namespace s21 {

ConsoleForWinograd::ConsoleForWinograd() : AbstractConsoleEngine() {
    start_message_ =
        "Hello. This program allows to test performance in "
        "Winograd matrix multiplication method and "
        "compare multithreading and single thread realisation.\n";
}

void ConsoleForWinograd::RequestParamsFromUser() {
    if (M1_) {
        delete M1_;
        M1_ = nullptr;
    }
    if (M2_) {
        delete M2_;
        M2_ = nullptr;
    }
    nmb_of_threads_ = 0;
    nmb_of_repeats_ = 0;
    rows_ = 0;
    cols_ = 0;

    cout << "Enter path to file with matrix, "
            "or you can enter integer N M and matrix will be filled randomly: ";
    while (!GetMatrixInput(&M1_)) {
        rows_ = 0;
        cols_ = 0;
    }
    cout << "First matrix has been load successfully, specify second matrix: ";

    while (!GetMatrixInput(&M2_)) {
        rows_ = 0;
        cols_ = 0;
    }
    cout << "Second matrix has been load successfully." << endl;

    if (M1_->get_cols() != M2_->get_rows()) {
        cout << "Columns of the first matrix should be equal rows of the second matrix\n" << endl;
        RequestParamsFromUser();
        return;
    } else {
        while ((nmb_of_repeats_ = RequestNmbFromUser("Enter number of repeats: ")) <= 0) {
            cout << "Invalid number of repeats, try again pls" << endl;
        }

        nmb_of_threads_ = RequestNmbFromUser("Enter number of threads for classic parallelism: ");
        while (nmb_of_threads_ <= 0 || nmb_of_threads_ % 2 != 0 || nmb_of_threads_ > 24) {
            cout << "Invalid number of thread,valid numbers are: 2, 4, 6, ... 24" << endl;
            nmb_of_threads_ = RequestNmbFromUser("Enter number of threads for classic parallelism: ");
        }
    }
    
    need_to_print_values_ = AskUserAboutPrintingValues();
}

void ConsoleForWinograd::RunAlgorithm() {
    cout << "Running algorithms..." << std::flush;
    S21Matrix result;

    if (need_to_print_values_) {
        cout << "First matrix:" << endl;
        S21Matrix::Print_matrix(*M1_);
        cout << "Second matrix:" << endl;
        S21Matrix::Print_matrix(*M2_);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nmb_of_repeats_; i++) {
        if (need_to_print_values_ && i == nmb_of_repeats_ - 1) {
            result = winograd_algorithm_.SolveWithoutParallelism(M1_, M2_);
        } else {
            winograd_algorithm_.SolveWithoutParallelism(M1_, M2_);
        }
    }

    if (need_to_print_values_) {
        cout << "\nResult matrix values from method without parallelism: " << endl;
        S21Matrix::Print_matrix(result);
    }

    duration_without_parallelism_ = std::chrono::high_resolution_clock::now() - start;

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nmb_of_repeats_; i++) {
        if (need_to_print_values_&& i == nmb_of_repeats_ - 1) {
            result = winograd_algorithm_.SolveWithPipelineParallelism(M1_, M2_);
        } else {
            result = winograd_algorithm_.SolveWithPipelineParallelism(M1_, M2_);
        }

    }

    duration_with_pipeline_parallelism_ = std::chrono::high_resolution_clock::now() - start;

    if (need_to_print_values_) {
        cout << "Result matrix values from method with pipeline parallelism: " << endl;
        S21Matrix::Print_matrix(result);
    }

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nmb_of_repeats_; i++) {
        if (need_to_print_values_ && i == nmb_of_repeats_ - 1) {
            result = winograd_algorithm_.SolveWithClassicParallelism(M1_, M2_, nmb_of_threads_);
        } else {
            winograd_algorithm_.SolveWithClassicParallelism(M1_, M2_, nmb_of_threads_);
        }
    }

    duration_with_classic_parallelism_ = std::chrono::high_resolution_clock::now() - start;

    if (need_to_print_values_) {
        cout << "Result matrix values from method with classic parallelism: " << endl;
        S21Matrix::Print_matrix(result);
    }

    cout << "Done" << endl;
}

void ConsoleForWinograd::PrintResult() {
    printf("Results:\n"
    "Duration without parallelism: %lfs\n"
    "Duration with pipeline parallelism: %lfs\n"
    "Duration with classic parallelism: %lfs\n\n", duration_without_parallelism_.count(),
                                                duration_with_pipeline_parallelism_.count(), 
                                                duration_with_classic_parallelism_.count());
}

bool ConsoleForWinograd::GetMatrixInput(S21Matrix **mat) {
    string input;
    std::getline(cin, input);
    char ch;

    if (sscanf(input.data(), "%d%c%d", &rows_, &ch, &cols_) == 3 && ch == ' ') {
        if (rows_ <= 0 || cols_ <= 0) {
            cout << "Invalid number of rows, cols, try again pls: ";
            return false;
        } else {
            *mat = new S21Matrix(rows_, cols_);
            S21Matrix::FillMatrixWithRandValues(*mat);
        }
    } else {
        fstream file(input);
        if (!file) {
            cout << "Invalid input, you need to enter file name or matrix dimensions(N M). Try again pls: ";
            return false;
        }
        *mat = S21Matrix::ParseFileWithMatrix(file);
        if (*mat == nullptr) {
            cout << "Error during parsing file, file has worng matrix dimensons or format, try again pls." << endl;
            cout << "Enter file name or matrix dimensions(N M): "; 
            return false;
        }
        S21Matrix::FillMatrixWithRandValues(*mat);
        file.close();
    }
    rows_ = 0;
    cols_ = 0;
    return true;
}

int ConsoleForWinograd::RequestNmbFromUser(string message) {
    std::string input;
    cout << message;
    std::getline(cin, input);
    int nmb;
    try {
        nmb = std::stoi(input);
    } catch (std::invalid_argument &exp) {
        nmb = 0;
    }
    return nmb;
}

bool ConsoleForWinograd::AskUserAboutPrintingValues() {
    if (M1_->get_cols() <= 0 || M2_->get_cols() <= 0 || M1_->get_rows() <= 0 || M2_->get_rows() <= 0) {
        return false;
    }

    string answer;
    cout << "Do you want to print matrix values (not recomended for big matrices)? y/n: ";
    std::getline(cin, answer);
    return answer.size() == 1 && (answer.at(0) == 'y' || answer.at(0) == 'Y');
}

}  // namespace s21