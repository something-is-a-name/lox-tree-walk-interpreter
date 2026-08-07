#include "LoxInstance.h"
#include "LoxFunction.h"
#include "LoxClass.h"

LoxInstance::LoxInstance(LoxClass* klass) :
	klass(std::move(klass)) {}

std::string LoxInstance::toString() const
{
	return klass->toString() + " instance";
}

std::any LoxInstance::get(Token name) {
	if (fields.contains(name.lexeme)) {
		return fields.at(name.lexeme);
	}

	LoxFunction* method = klass->findMethod(name.lexeme);
	if (method != nullptr) return method->bind(shared_from_this());


	throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, std::any value)
{
	fields[name.lexeme] = value;
}
