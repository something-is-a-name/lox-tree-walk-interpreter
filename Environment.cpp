#include "Environment.h"

Environment::Environment(Environment* enclosing) :
	enclosing(enclosing) {}


void Environment::define(std::string name, std::any value) {
	values[name] = value;
}

Environment* Environment::ancestor(int distance)
{
	Environment* environment = this;

	for (int i = 0; i < distance; i++) {
		environment = environment->enclosing;
	}

	return environment;
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

void Environment::assignAt(int distance, Token name, std::any value)
{
	ancestor(distance)->values[name.lexeme] = value;
}

std::any Environment::get(Token name) {
	if (values.contains(name.lexeme)) {
		return values.at(name.lexeme);
	}

	if (enclosing != nullptr) return enclosing->get(name);

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "' .");
}

std::any Environment::getAt(int distance, std::string name)
{
	return ancestor(distance)->values.at(name);
}
