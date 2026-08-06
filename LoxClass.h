#pragma once
#include <string>
#include <map>
#include "LoxCallable.h"
#include "LoxFunction.h"


class LoxClass : public LoxCallable {
public:
	LoxClass(std::string name, LoxClass* superclass, std::map<std::string, LoxFunction> methods);

	int arity()  override;

	std::string toString() const override;

	LoxFunction* findMethod(std::string name);

	std::any call(Interpreter& interpreter,
		const std::vector<std::any> arguments) override;
private:
	std::string name;
	LoxClass* superclass;
	std::map<std::string, LoxFunction> methods;
};
