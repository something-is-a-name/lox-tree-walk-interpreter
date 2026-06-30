#pragma once
#include <any>
#include <vector>

class Interpreter; // forward declaration

class LoxCallable {
public:
    virtual ~LoxCallable() = default;

    // how many arguments it expects
    virtual int arity() const = 0;

    virtual std::any call(Interpreter& interpreter,
        const std::vector<std::any>& arguments) = 0;
};