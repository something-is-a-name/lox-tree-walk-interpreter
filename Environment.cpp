#include "Environment.h"

Environment::Environment(Environment* enclosing) :
	enclosing(enclosing) {}


void Environment::define(std::string name, std::any value) {
	values[name] = value;
}

void Environment::assign(Token name, std::any value) {
	if (values.contains(name.lexeme)) {
		values[name.lexeme] = std::move(value);
		return;
	}

	if (enclosing != nullptr) {
		enclosing->assign(name, std::move(value));
		return;
	}

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "' .");
}

std::any Environment::get(Token name) {
	if (values.contains(name.lexeme)) {
		return values.at(name.lexeme);
	}

	if (enclosing != nullptr) return enclosing->get(name);

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "' .");
}