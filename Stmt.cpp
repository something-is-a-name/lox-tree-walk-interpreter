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

// If

If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch) :
	condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}


std::any If::accept(StmtVisitor& visitor) const {
	return visitor.visitIfStmt(*this);
}

// While

While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) :
	condition(std::move(condition)), body(std::move(body)) {}


std::any While::accept(StmtVisitor& visitor)const {
	return visitor.visitWhileStmt(*this);
}

// Function

Function::Function(Token name, std::vector<std::unique_ptr<Expr>> params, std::vector<std::unique_ptr<Stmt>> body):
	name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

std::any Function::accept(StmtVisitor& visitor) const {
	return visitor.visitFunctionStmt(*this);
}