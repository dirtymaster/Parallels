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
    TsmResult multithread_result_, single_thread_result_;

    std::chrono::duration<double> duration_without_parallelism_;
    std::chrono::duration<double> duration_with_parallelism_;

    void ShowData(double time, TsmResult &path);
    int InputValue(int bottom_border);
    void WrongInputMessage();
    void RequestParamsFromUser();
    void RunAlgorithm();
    void PrintResult();
};
}  // namespace s21

#endif  // PARALLELS_CONSOLEFORANT_H
