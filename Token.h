#pragma once

#include "TokenType.h"
#include <string>
#include <any>

class Token {
public:
	Token(TokenType type, std::string lexeme, std::any literal, int line);
	std::string toString() const;


	TokenType type;
	std::string lexeme;
	std::any literal;
	int line;
};