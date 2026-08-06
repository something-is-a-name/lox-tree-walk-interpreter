#include "LoxClass.h"
#include "LoxInstance.h"

LoxClass::LoxClass(std::string name , LoxClass* superclass, std::map<std::string, LoxFunction> methods) :
	name(std::move(name)), superclass(superclass), methods(std::move(methods)) {}

int LoxClass::arity() 
{
    LoxFunction* initializer = findMethod("init");
    if (initializer == nullptr) return 0;

	return initializer->arity();
}

std::string LoxClass::toString() const
{
	return name;
}

LoxFunction* LoxClass::findMethod(std::string name) 
{
    auto it = methods.find(name);
    if (it != methods.end()) {
        return &it->second;
    }

    if (superclass != nullptr) {
        return superclass->findMethod(name);
    }

    return nullptr;
}

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any> arguments)
{
	LoxInstance instance(this);

    LoxFunction* initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance).call(interpreter, arguments);
    }

    return instance;

    
}
