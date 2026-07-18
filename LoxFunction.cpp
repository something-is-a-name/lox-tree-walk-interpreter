#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Interpreter.h"
#include "Stmt.h"

LoxFunction::LoxFunction(const Function& declaration,
    Environment* closure, bool isInitializer)
    : declaration(&declaration), closure(std::move(closure)), isInitializer(isInitializer)  
{}

int LoxFunction::arity()  {
	return declaration->params.size();
}

std::string LoxFunction::toString() const {
	return "<fn" + declaration->name.lexeme + ">";
}

std::any LoxFunction::call(Interpreter& interpreter,
    std::vector<std::any> arguments) {

    Environment functionEnv(*closure);

    for (int i = 0; i < declaration->params.size(); i++) {
        functionEnv.define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, &functionEnv);
    }
    catch (const Return& returnValue) {
        if (isInitializer) return closure->getAt(0, "this");
        return returnValue.value;
    }

    if (isInitializer) return closure->getAt(0, "this");
    return nullptr;
}

LoxFunction LoxFunction::bind(const LoxInstance& instance)
{
    Environment environment(closure);
    environment.define("this", instance);
    return LoxFunction(*declaration, &environment, isInitializer);
}
