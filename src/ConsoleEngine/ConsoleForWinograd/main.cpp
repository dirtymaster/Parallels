#include "ConsoleForWinograd.h"

int main() {
    s21::AbstractConsoleEngine *console = new s21::ConsoleForWinograd();
    console->start();
    return 0;
}