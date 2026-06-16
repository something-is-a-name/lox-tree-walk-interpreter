#include "Token.h"

class Token {

public:
	TokenType type;
	const std::string lexeme;
	const std::any literal;
	const int line;

	Token(TokenType type, std::string lexeme, std::any literal, int line) :
		type{ type }, lexeme{ std::move(lexeme) }, literal{ std::move(literal) }, line{ line }
	{}

	std::string toString() const {

		std::string literal_new = "nil";

		if (literal.has_value()) {
			if (literal.type() == typeid(double)) {
				literal_new = std::to_string(std::any_cast<double>(literal));
			}
			else if (literal.type() == typeid(std::string)) {
				literal_new = std::any_cast<std::string>(literal);
			}
			else if (literal.type() == typeid(bool)) {
				literal_new = std::any_cast<bool>(literal) ? "true" : "false";
			}
		}

		return tokenTypeToString(type) + " " + lexeme + " " + literal_new;
	}
};