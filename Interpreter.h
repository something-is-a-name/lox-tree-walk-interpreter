#pragma once
#include "Expr.h"
#include "RuntimeError.h"
#include <iostream>
#include "Stmt.h"
#include "Environment.h"
#include <sstream>
#include "LoxFunction.h"
#include "Return.h"
#include "LoxClass.h"

class Lox; 

class Interpreter : public ExprVisitor, public StmtVisitor {

public:

	Interpreter();

	void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

	Environment* getGlobals() const;

	std::any visitLiteralExpr(const Literal& expr) override;

	std::any visitGroupingExpr(const Grouping& expr) override;

	std::any visitUnaryExpr(const Unary& expr) override;

	std::any visitBinaryExpr(const Binary& expr) override;

	std::any visitCommaExpr(const Comma& expr) override;

	std::any visitTernaryExpr(const Ternary& expr) override;

	std::any visitAssignExpr(const Assign& expr) override;

	std::any visitVariableExpr(const Variable& expr) override;

	std::any visitLogicalExpr(const Logical& expr) override;

	std::any visitCallExpr(const Call& expr) override;

	std::any visitExpressionStmt(const Expression& stmt) override;

	std::any visitPrintStmt(const Print& stmt) override;

	std::any visitVarStmt(const Var& stmt) override;

	std::any visitBlockStmt(const Block& stmt) override;

	std::any visitIfStmt(const If& stmt) override;

	std::any visitWhileStmt(const While& stmt) override;

	std::any visitFunctionStmt(const Function& stmt) override;

	std::any visitReturnStmt(const ReturnStmt& stmt) override;

	std::any visitClassStmt(const Class& stmt) override;


	void executeBlock(const std::vector<std::unique_ptr<Stmt>>&  statements, Environment* environment);

	void resolve(const Expr& expr, int depth);

private:

	Environment* globals;
	Environment* environment;

	std::map<const Expr*, int > locals {};

	std::any lookUpVariable(Token name, const Expr& expr);

	std::any evaluate(const Expr& expr);

	void execute(const Stmt& stmt);

	bool  isTruthy(std::any& v);

	bool isEqual(std::any& a, std::any& b);

	double asNumber(const std::any& v);

	bool asBool(const std::any& v);

	std::string asString(const std::any& v);

	void checkNumberOperand(Token op, std::any& operand);

	void checkNumberOperands(Token op, std::any& left, std::any& right);

	std::string stringify(const std::any& v);
};