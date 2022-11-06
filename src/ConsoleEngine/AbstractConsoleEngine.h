
#ifndef PARALLELS_ABSTRACTCONSOLEENGINE_H
#define PARALLELS_ABSTRACTCONSOLEENGINE_H

#include <iostream>
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::cin;
using std::string;

namespace s21 {
class AbstractConsoleEngine {
public:
    AbstractConsoleEngine() {}
    ~AbstractConsoleEngine() {}

    void start() {
        cout << start_message_ << endl;
        RequestParamsFromUser();
        RunAlgorithm();
        PrintResult();
    }

protected:
    string start_message_;
    virtual void RequestParamsFromUser() = 0;
    virtual void RunAlgorithm() = 0;
    virtual void PrintResult() = 0;
};

}  // namespace s21

#endif  // PARALLELS_ABSTRACTCONSOLEENGINE_H
