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
#ifdef ANT
    s21::ConsoleForAnt console;
#endif
#ifdef GAUSS
    s21::ConsoleForGauss console;
#endif
#ifdef WINOGRAD
    s21::ConsoleForWinograd console;
#endif
    console.start();
    return 0;
}