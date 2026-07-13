#pragma once
#include "LoxClass.h"4
#include "Token.h"
#include <map>
#include "RuntimeError.h"

class LoxInstance {
public:
	LoxInstance(LoxClass klass);

	std::string toString() const;

	std::any get(Token name) {
		if (fields.contains(name.lexeme)) {
			return fields.at(name.lexeme);
		}

		throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
	}
private:
	LoxClass klass;
	std::map<std::string, std::any> fields {};
};