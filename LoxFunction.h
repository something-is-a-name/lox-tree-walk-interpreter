#pragma once
#include "LoxCallable.h"
#include <memory>

class Interpreter; 

class Function;


class LoxFunction : public LoxCallable {
public:
	LoxFunction(const Function& declaration);

	int arity() const override;

	std::string toString() const override;

	std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
private:
	const Function* declaration;
};
