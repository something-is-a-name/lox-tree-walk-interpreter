#include "LoxCallable.h"


int ClockCallable::arity() const  {
        return 0;
    }

std::any ClockCallable::call(Interpreter& interpreter,
    const std::vector<std::any> arguments)  {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto seconds = duration<double>(now.time_since_epoch()).count();
    return seconds;
}

std::string ClockCallable::toString() const  {
    return "<native fn>";
}
