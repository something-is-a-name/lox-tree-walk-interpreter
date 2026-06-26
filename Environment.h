#pragma once
#include <map>
#include <string>
#include <any>
#include "RuntimeError.h"
#include "Token.h"

class Environment {
public:
	Environment(Environment* enclosing);
	Environment() = default;

	void define(std::string name, std::any value);

	std::any get(Token name);

	void assign(Token name, std::any value);

private:
	std::map<std::string, std::any> values {};
	Environment* enclosing{ nullptr };
};

