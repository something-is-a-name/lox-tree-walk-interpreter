#pragma once
#include "Expr.h"
#include "Stmt.h"
#include  <map>
#include <stack>

class Lox;
class Interpreter;

class Resolver : public ExprVisitor, public StmtVisitor {
public:
	Resolver(Interpreter& interpreter);

	std::any visitBlockStmt(const Block& stmt) override;

	std::any visitVarStmt(const Var& stmt) override;

	std::any visitFunctionStmt(const Function& stmt) override;

	std::any visitReturnStmt(const ReturnStmt& stmt) override;

	std::any visitWhileStmt(const While& stmt) override;

	std::any visitIfStmt(const If& stmt) override;

	std::any visitPrintStmt(const Print& stmt) override;

	std::any visitExpressionStmt(const Expression& stmt) override;

	std::any visitVariableExpr(const Variable& expr) override;

	std::any visitAssignExpr(const Assign& expr) override;

	std::any visitBinaryExpr(const Binary& expr) override;

	std::any visitCallExpr(const Call& expr) override;

	std::any visitGroupingExpr(const Grouping& expr) override;

	std::any visitLiteralExpr(const Literal& expr) override;
	
	std::any visitLogicalExpr(const Logical& expr) override;

	std::any visitUnaryExpr(const Unary& expr) override;

	std::any visitCommaExpr(const Comma& expr) override;

	std::any visitTernaryExpr(const Ternary& expr) override;

	void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);
	void resolve(const std::vector<std::unique_ptr<Expr>>& statements);
	void resolve(const Stmt& stmt);
	void resolve(const Expr& expr);

private:


	
	void beginScope();
	void endScope();

	void declare(Token name);
	void define(Token name);

	void resolveLocal(const Expr& expr, Token name);
	void resolveFunction(const Function& function);

	 Interpreter& interpreter;

	std::vector<std::map<std::string, bool>> scopes {};
};