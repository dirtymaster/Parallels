#ifndef PARALLELS_CONSOLEFORANT_H
#define PARALLELS_CONSOLEFORANT_H

#include "../../Algorithms/AntColonyAlgorithm/AntAlgorithm.h"
#include "../AbstractConsoleEngine.h"

namespace s21 {
class ConsoleForAnt : public AbstractConsoleEngine {
public:
    ConsoleForAnt();

private:
    AntAlgorithm ant_solver_;
    bool correct_data;

    void RequestParamsFromUser();
    void RunAlgorithm();
    void PrintResult();
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEFORANT_H
