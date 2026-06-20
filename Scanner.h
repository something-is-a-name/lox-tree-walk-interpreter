#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <map>


class Scanner {
public:
	explicit Scanner(std::string source);

	std::vector<Token> scanTokens();

	

private:


	std::string source;
	std::vector<Token> tokens {};

	int start{ 0 };
	int current{ 0 };
	int line{ 1 };

	const std::map<std::string, TokenType> keywords {
		{"and", AND},
		{ "class", CLASS },
		{ "else", ELSE },
		{ "false", FALSE },
		{ "for", FOR },
		{ "fun", FUN },
		{ "if", IF },
		{ "nil", NIL },
		{ "or", OR },
		{ "print", PRINT },
		{ "return", RETURN },
		{ "super", SUPER },
		{ "this", THIS },
		{ "true", TRUE },
		{ "var", VAR },
		{ "while", WHILE }

	};

	char advance();
	bool isAtEnd();
	void addToken(TokenType type, std::any literal);
	void addToken(TokenType type);
	bool match(char expected);
	char peek();
	char peekNext();
	void string();
	bool isDigit(char c);
	void number();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
	void identifier();
	void scanToken();



};