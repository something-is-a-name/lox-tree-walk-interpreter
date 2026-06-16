#include "Expr.h"
#include <vector> 
#include <stdexcept>
#include "Lox.h"

class Parser {
private:

	class ParseError : public std::runtime_error {

	public:
		ParseError(const std::string& message)
			: std::runtime_error(message) {}
};



	std::vector<Token> tokens {};
	int current{ 0 };

	template<typename... TokenTypes>

	std::unique_ptr<Expr> parse() {
		try {
			return expression();
		}
		catch (ParseError error) {
			return NULL;
		}
	}

	bool match(std::initializer_list<TokenType> types) {
		for (TokenType type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}
		return false;
	}

	Token consume(TokenType type, const std::string& message) {
		if (check(type)) return advance();

		throw error(peek(), message);
	}

	ParseError error(Token token, const std::string& message) {
		Lox::error(token, message);
		ParseError error = ParseError(message);
		return error;
	}

	void synchonize() {
		advance();

		while (!isAtEnd()) {
			if (previous().type == SEMICOLON) return;

			switch (peek().type) {
				case CLASS:
				case FUN:
				case VAR:
				case FOR:
				case IF:
				case WHILE:
				case PRINT:
				case RETURN:
					return;
			}

			advance();
		}
	}

	bool check(TokenType type)  {
		if (isAtEnd()) return false;
		return peek().type == type;
	}

	Token advance() {
		if (!isAtEnd()) current++;
		return previous();
	}

	bool isAtEnd() {
		return peek().type == EoF;
	}
	 
	Token peek()  {
		return tokens.at(current);
	}

	Token previous() {
		return tokens.at(current - 1);
	}

	std::unique_ptr<Expr> expression() {
		return comma();
	}

	std::unique_ptr<Expr> ternary() {
		auto expr = equality();

		if (match({ QUESTION })) {
			auto thenExpr = expression();

			consume(COLON, "expected ':' after '?' ");
			
			auto elseExpr = ternary();

			expr = std::make_unique<Ternary>(std::move(expr), std::move(thenExpr), std::move(elseExpr));
		}

		return expr;
		
	}

	std::unique_ptr<Expr> comma() {

		std::vector<std::unique_ptr<Expr>> exprs {};

		exprs.push_back(ternary());

		while (match({COMMA})) {
			auto expr = ternary();
			exprs.push_back(expr);
		}

		return std::make_unique<Comma>(std::move(exprs));
	}

	std::unique_ptr<Expr> equality() {
		auto expr = comparison();

		while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
			Token op = previous();
			auto right = comparison();
			expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
		}

		return expr;
	}

	std::unique_ptr<Expr> comparison() {
		auto expr = term();

		while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
			Token op = previous();
			auto right = term();
			expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
		}

		return expr;
	}

	std::unique_ptr<Expr> term() {
		auto expr = factor();

		while (match({ MINUS, PLUS})) {
			Token op = previous();
			auto right = factor();
			expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
		}

		return expr;

	}

	std::unique_ptr<Expr> factor() {
		auto expr = unary();

		while (match({ SLASH, STAR })) {
			Token op = previous();
			auto right = unary();
			expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
		}

		return expr;

	}

	std::unique_ptr<Expr> unary() {

		if (match({ BANG, MINUS })) {
			Token op = previous();
			auto right = unary();
			return  std::make_unique<Unary>(op, std::move(right));
		}

		return primary();

	}

	std::unique_ptr<Expr> primary() {
		if (match({ FALSE })) return std::make_unique<Literal>(false);
		if (match({ TRUE })) return std::make_unique<Literal>(true);
		if (match({ NIL })) return std::make_unique<Literal>(NULL);

		if (match({ NUMBER, STRING })) {
			return std::make_unique<Literal>(previous().literal);
		}

		if (match({ LEFT_PAREN })) {
			std::unique_ptr<Expr> expr = expression();
			consume(RIGHT_PAREN, "Expected ')' after expression.");
			return std::make_unique<Grouping>(expr);
		}

		throw error(peek(), "Expected expression.");
	}


public:
	Parser(std::vector<Token> tokens) :
		tokens(std::move(tokens)) {}

};