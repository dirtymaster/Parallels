#ifndef PARALLELS_CONSOLEFORANT_H
#define PARALLELS_CONSOLEFORANT_H

#include "../AbstractConsoleEngine.h"
#include "../../Algorithms/AntColonyAlgorithm/AntAlgorithm.h"

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
}


#endif //PARALLELS_CONSOLEFORANT_H
