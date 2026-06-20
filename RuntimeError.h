#pragma once
#include <string>
#include <stdexcept>
#include "Token.h"

class RuntimeError : public std::runtime_error {

public:
	RuntimeError(Token token, const std::string& message)
		: token(std::move(token)), std::runtime_error(message) {}

	Token token;
};
