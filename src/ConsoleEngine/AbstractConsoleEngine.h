
#ifndef PARALLELS_ABSTRACTCONSOLEENGINE_H
#define PARALLELS_ABSTRACTCONSOLEENGINE_H

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

namespace s21 {
class AbstractConsoleEngine {
public:
    AbstractConsoleEngine() {}
    ~AbstractConsoleEngine() {}

    void start() {
        cout << start_message_ << endl;
        RequestParamsFromUser();
        RunAlgorithms();
        PrintResult();
    }

protected:
    string start_message_;
    virtual void RequestParamsFromUser() = 0;
    virtual void RunAlgorithms() = 0;
    virtual void PrintResult() = 0;
};

}

#endif //PARALLELS_ABSTRACTCONSOLEENGINE_H
