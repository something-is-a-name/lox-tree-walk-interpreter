#include "LoxFunction.h"
#include "Interpreter.h"
#include "Stmt.h"

LoxFunction::LoxFunction(const Function& declaration) :
	declaration(&declaration) {}

int LoxFunction::arity() const {
	return declaration->params.size();
}

std::string LoxFunction::toString() const {
	return "<fn" + declaration->name.lexeme + ">";
}

std::any LoxFunction::call(Interpreter& interpreter, std::vector<std::any> arguments) {
	Environment* environment{ interpreter.getGlobals()};

	for (int i = 0; i < declaration->params.size(); i++) {
		environment->define(declaration->params[i].lexeme, arguments[i]);
	}

	interpreter.executeBlock(declaration->body, environment);
	return nullptr;

}