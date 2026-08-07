#pragma once
#include "LoxCallable.h"
#include <memory>
#include "Return.h"
#include "Environment.h"

class Interpreter; 
class Function;
class LoxInstance;


class LoxFunction : public LoxCallable {
public:
	LoxFunction(const Function& declaration,
		Environment* closure, bool isInitializer);

	int arity()  override;

	std::string toString() const override;

	std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;

	LoxFunction bind(std::shared_ptr<LoxInstance> instance);

private:
	const Function* declaration;

	Environment* closure;

	bool isInitializer;
};

