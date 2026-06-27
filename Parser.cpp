#include "Parser.h"


Parser::ParseError::ParseError(const std::string& message) : std::runtime_error(message) {}


template<typename... TokenTypes>
bool Parser::match(std::initializer_list<TokenType> types) {
	for (TokenType type : types) {
		if (check(type)) {
			advance();
			return true;
		}
	}
	return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
	if (check(type)) return advance();

	throw error(peek(), message);
}


Parser::ParseError Parser::error(Token token, const std::string& message) {
	Lox::error(token, message);
	return ParseError(message);
}


void Parser::synchonize() {
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

bool Parser::check(TokenType type)  {
	if (isAtEnd()) return false;
	return peek().type == type;
}

Token Parser::advance() {
	if (!isAtEnd()) current++;
	return previous();
}

bool Parser::isAtEnd() {
	return peek().type == EoF;
}
	 
Token Parser::peek()  {
	return tokens.at(current);
}

Token Parser::previous() {
	return tokens.at(current - 1);
}

std::unique_ptr<Stmt> Parser::statement() {
	if (match({ IF }))  return ifStatement();
	if (match({ PRINT })) return printStatement();
	if (match({ LEFT_BRACE })) return std::make_unique<Block>(block());

	return expressionStatement();


}

std::unique_ptr<Stmt> Parser::printStatement() {
	auto value = expression();
	consume(SEMICOLON, "Expected ';' after value.");

	return std::make_unique<Print>(std::move(value));

}

std::unique_ptr<Stmt> Parser::varDeclaration() {
	Token name = consume(IDENTIFIER, "Expected variable name.");

	std::unique_ptr<Expr> initializer = nullptr;

	if (match({ EQUAL })) {
		initializer = expression();
	}

	consume(SEMICOLON, "Expected ';' after variable declaration.");
	return std::make_unique<Var>(std::move(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::ifStatement() {
	consume(LEFT_PAREN, "Expected  '('  after 'if' .");
	auto condition = expression();
	consume(RIGHT_PAREN, "Expected  ')'  after if condition.");

	auto thenBranch = statement();
	std::unique_ptr<Stmt> elseBranch { nullptr };
	if (match({ ELSE })) {
		elseBranch = statement();
	}

	return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));

}


std::unique_ptr<Stmt> Parser::expressionStatement() {
	auto expr = expression();
	consume(SEMICOLON, "Expected ';' after expression.");

	return std::make_unique<Expression>(std::move(expr));

}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
	std::vector<std::unique_ptr<Stmt>> statements {};

	while (!check(RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(declaration());
	}

	consume(RIGHT_BRACE, "Expected '}' after block.");
	return statements;
}

std::unique_ptr<Expr> Parser::expression() {
	return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {
	auto expr = comma();

	if (match({ EQUAL })) {
		Token equals = previous();
		auto value = assignment();

		if (auto* variable = dynamic_cast<Variable*>(expr.get())) {
			Token name = variable->name;
			return std::make_unique<Assign>(std::move(name), std::move(value));
		}

		Lox::error(equals, "Invalid assignment target.");

	}

	return expr;
}

std::unique_ptr<Stmt> Parser::declaration() {
	try {
		if (match({ VAR })) return varDeclaration();

		return statement();
	}
	catch (ParseError error) {
		synchonize();
		return nullptr;
	}
}


std::unique_ptr<Expr> Parser::comma() {

	std::vector<std::unique_ptr<Expr>> exprs {};

	exprs.push_back(ternary());

	while (match({ COMMA })) {
		auto expr = ternary();
		exprs.push_back(std::move(expr));
	}

	return std::make_unique<Comma>(std::move(exprs));
}

std::unique_ptr<Expr> Parser::ternary() {
	auto expr = equality();

	if (match({ QUESTION })) {
		auto thenExpr = expression();

		consume(COLON, "expected ':' after '?' ");
			
		auto elseExpr = ternary();

		expr = std::make_unique<Ternary>(std::move(expr), std::move(thenExpr), std::move(elseExpr));
	}

	return expr;
		
}

std::unique_ptr<Expr> Parser::equality() {
	auto expr = comparison();

	while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
		Token op = previous();
		auto right = comparison();
		expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
	auto expr = term();

	while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
		Token op = previous();
		auto right = term();
		expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
	}

	return expr;
}

std::unique_ptr<Expr> Parser::term() {
	auto expr = factor();

	while (match({ MINUS, PLUS})) {
		Token op = previous();
		auto right = factor();
		expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
	}

	return expr;

}

std::unique_ptr<Expr> Parser::factor() {
	auto expr = unary();

	while (match({ SLASH, STAR })) {
		Token op = previous();
		auto right = unary();
		expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
	}

	return expr;

}

std::unique_ptr<Expr> Parser::unary() {

	if (match({ BANG, MINUS })) {
		Token op = previous();
		auto right = unary();
		return  std::make_unique<Unary>(op, std::move(right));
	}

	return primary();

}

std::unique_ptr<Expr> Parser::primary() {
	if (match({ FALSE })) return std::make_unique<Literal>(false);
	if (match({ TRUE })) return std::make_unique<Literal>(true);
	if (match({ NIL })) return std::make_unique<Literal>(nullptr);

	if (match({ NUMBER, STRING })) {
		return std::make_unique<Literal>(previous().literal);
	}

	if (match({ IDENTIFIER })) {
		return std::make_unique<Variable>(previous());
	}

	if (match({ LEFT_PAREN })) {
		std::unique_ptr<Expr> expr = expression();
		consume(RIGHT_PAREN, "Expected ')' after expression.");
		return std::make_unique<Grouping>(std::move(expr));
	}

	throw error(peek(), "Expected expression.");
}



Parser::Parser(std::vector<Token> tokens) :
	tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {

	std::vector<std::unique_ptr<Stmt>> statements {};
	while (!isAtEnd()) {
		statements.push_back(declaration());
	}

	return statements;
}
