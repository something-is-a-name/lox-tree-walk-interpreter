#include "TokenType.h"

const std::string& tokenTypeToString(TokenType type) {
	static const std::string names[] = {
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",  "LEFT_BRACKET", "RIGHT_BRACKET",
		"COMMA", "DOT", "MINUS", "PLUS", "COLON", "SEMICOLON", "SLASH", "STAR", "QUESTION",

		"BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
		"GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",

		"IDENTIFIER", "STRING", "NUMBER",

		"AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
		"PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

		"EOF"
	};

	return names[static_cast<int>(type)];
}