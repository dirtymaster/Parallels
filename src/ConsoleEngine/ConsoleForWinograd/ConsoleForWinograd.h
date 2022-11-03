
#ifndef PARALLELS_CONSOLEFORWINOGRAD_H
#define PARALLELS_CONSOLEFORWINOGRAD_H

#include "../AbstractConsoleEngine.h"

namespace s21 {

class ConsoleForWinograd : public AbstractConsoleEngine {

public:
    ConsoleForWinograd();

private:
    void RequestParamsFromUser();
    void RunAlgorithms();
    void PrintResult();

    // In private fields use whatever you want for algorithm
};

}  // namespace s21

#endif //PARALLELS_CONSOLEFORWINOGRAD_H
