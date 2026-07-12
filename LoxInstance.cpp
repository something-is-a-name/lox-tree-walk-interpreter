#include "LoxInstance.h"

LoxInstance::LoxInstance(LoxClass klass) :
	klass(std::move(klass)) {}

std::string LoxInstance::toString() const
{
	return klass.toString() + " instance";
}
