#include "LoxClass.h"

LoxClass::LoxClass(std::string name) :
	name(std::move(name)) {}

int LoxClass::arity() const
{
	return 0;
}

std::string LoxClass::toString() const
{
	return name;
}

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any> arguments)
{
	LoxInstance instance(*this);
}
