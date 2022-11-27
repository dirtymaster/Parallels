#ifdef ANT
#include "ConsoleForAnt/ConsoleForAnt.h"
#endif
#ifdef GAUSS
#include "ConsoleForGauss/ConsoleForGauss.h"
#endif
#ifdef WINOGRAD
#include "ConsoleForWinograd/ConsoleForWinograd.h"
#endif

int main() {
    s21::AbstractConsoleEngine *console;
    // if constexpr ANT {
    //     console = new s21::ConsoleForAnt;
    //     // s21::ConsoleForAnt console;
    // }
#ifdef ANT
    console = new s21::ConsoleForAnt;
    // s21::ConsoleForAnt console;
#endif
#ifdef GAUSS
    console = new s21::ConsoleForGauss;
    // s21::ConsoleForGauss console;
#endif
#ifdef WINOGRAD
    console = new s21::ConsoleForWinograd;
    // s21::ConsoleForWinograd console;
#endif
    console->start();
    delete console;
    return 0;
}
