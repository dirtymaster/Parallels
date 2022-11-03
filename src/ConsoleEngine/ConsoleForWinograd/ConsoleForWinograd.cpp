
#include "ConsoleForWinograd.h"

namespace s21 {

ConsoleForWinograd::ConsoleForWinograd() : AbstractConsoleEngine() {
    start_message_ = "Starting message for Winograd ALgo";
}

void ConsoleForWinograd::RequestParamsFromUser() {
    cout << "Here we should ask user for required params,"
            "validate them and store to private fields" << endl;
}

void ConsoleForWinograd::RunAlgorithms() {
    cout << "Here we invoke algorithms" << endl;
}

void ConsoleForWinograd::PrintResult() {
    cout << "Output results" << endl;
}

}