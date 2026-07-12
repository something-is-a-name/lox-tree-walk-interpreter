#pragma once
#include <string>
#include "LoxCallable.h"

class LoxClass : public LoxCallable {
public:
	LoxClass(std::string name);

	int arity() const override;

	std::string toString() const override;

	std::any call(Interpreter& interpreter,
		const std::vector<std::any> arguments) override;
private:
	std::string name;
};
