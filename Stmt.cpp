#include "Stmt.h"

// Stmt

Stmt::~Stmt() = default;

// Block


Block::Block(std::vector<std::unique_ptr<Stmt>> statements) :
	statements(std::move(statements)) {}


std::any Block::accept(StmtVisitor& visitor) const {
	return visitor.visitBlockStmt(*this);
}


// Expression

Expression::Expression(std::unique_ptr<Expr> expression) :
	expression(std::move(expression)) {}

std::any Expression::accept(StmtVisitor& visitor)const {
	return visitor.visitExpressionStmt(*this);
}

// Print

Print::Print(std::unique_ptr<Expr> expression) :
	expression(std::move(expression)) {}

std::any Print::accept(StmtVisitor& visitor) const {
	return visitor.visitPrintStmt(*this);
}


Var::Var(Token name, std::unique_ptr<Expr> initializer) :
	name(std::move(name)), initializer(std::move(initializer)) {}

std::any Var::accept(StmtVisitor& visitor) const {
	return visitor.visitVarStmt(*this);
}

