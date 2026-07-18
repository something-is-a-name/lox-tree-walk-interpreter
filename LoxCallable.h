#pragma once
#include <any>
#include <vector>
#include <chrono>
#include <string>

class Interpreter; // forward declaration
class ClockCallable;

class LoxCallable {
public:
    virtual ~LoxCallable() = default;

    // how many arguments it expects
    virtual int arity() = 0;

    virtual std::string toString() const = 0;

    virtual std::any call(Interpreter& interpreter,
        const std::vector<std::any> arguments) = 0;
};

class ClockCallable : public LoxCallable {
public:
    int arity()  override;

    std::any call(Interpreter& interpreter,
        const std::vector<std::any> arguments) override;

    std::string toString() const override;
};