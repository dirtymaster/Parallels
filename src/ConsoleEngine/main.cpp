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
#ifdef ANT
    console = new s21::ConsoleForAnt;
#endif
#ifdef GAUSS
    console = new s21::ConsoleForGauss;
#endif
#ifdef WINOGRAD
    console = new s21::ConsoleForWinograd;
#endif
    console->start();
    delete console;
    return 0;
}
