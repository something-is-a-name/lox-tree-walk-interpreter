#pragma once
#include "LoxCallable.h"
#include <memory>
#include "Return.h"
#include "Environment.h"

class Interpreter; 

class Function;


class LoxFunction : public LoxCallable {
public:
	LoxFunction(const Function& declaration,
		Environment* closure);

	int arity() const override;

	std::string toString() const override;

	std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
private:
	const Function* declaration;

	Environment* closure;
};

