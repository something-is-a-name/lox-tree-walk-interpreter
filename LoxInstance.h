#pragma once
#include "Token.h"
#include <map>
#include "RuntimeError.h"

class LoxClass;

class LoxInstance {
public:
	LoxInstance(LoxClass* klass);

	std::string toString() const;

	std::any get(Token name);

	void set(Token name, std::any value);
private:
	LoxClass* klass;
	std::map<std::string, std::any> fields {};
};