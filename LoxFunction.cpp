#include "LoxFunction.h"
#include "Interpreter.h"
#include "Stmt.h"

LoxFunction::LoxFunction(const Function& declaration,
    Environment* closure)
    : declaration(&declaration), closure(std::move(closure))
{}

int LoxFunction::arity() const {
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
        return returnValue.value;
    }

    return nullptr;
}