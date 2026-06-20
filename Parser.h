#pragma once
#include "Expr.h"
#include <vector> 
#include <stdexcept>
#include "Lox.h"

class Parser {
private:

	class ParseError : public std::runtime_error {

	public:
		ParseError(const std::string& message);
	};


	std::vector<Token> tokens {};
	int current{ 0 };
	template<typename... TokenTypes>

	bool match(std::initializer_list<TokenType> types);

	Token consume(TokenType type, const std::string& message);

	ParseError error(Token token, const std::string& message);

	void synchonize();

	bool check(TokenType type);

	Token advance();

	bool isAtEnd();

	Token peek();

	Token previous();

	std::unique_ptr<Expr> expression();

	std::unique_ptr<Expr> comma();

	std::unique_ptr<Expr> ternary();

	std::unique_ptr<Expr> equality();
	
	std::unique_ptr<Expr> comparison();

	std::unique_ptr<Expr> term();

	std::unique_ptr<Expr> factor();

	std::unique_ptr<Expr> unary();
	
	std::unique_ptr<Expr> primary();


public:
	Parser(std::vector<Token> tokens);

	std::unique_ptr<Expr> parse();

};