#include "Environment.h"

void Environment::define(std::string name, std::any value) {
	values.insert({ name, value });
}

void Environment::assign(Token name, std::any value) {
	if (values.contains(name.lexeme)) {
		values.insert({ name.lexeme, value });
		return;
	}

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "' .");
}

std::any Environment::get(Token name) {
	if (values.contains(name.lexeme)) {
		return values.at(name.lexeme);
	}

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "' .");
}