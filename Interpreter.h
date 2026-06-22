#include "Expr.h"
#include "RuntimeError.h"
#include <iostream>
#include "Stmt.h"
#include <sstream>

class Lox; 

class Interpreter : public ExprVisitor, public StmtVisitor {

public:
	Interpreter() {}
	void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

	std::any visitLiteralExpr(const Literal& expr) override;

	std::any visitGroupingExpr(const Grouping& expr) override;

	std::any visitUnaryExpr(const Unary& expr) override;

	std::any visitBinaryExpr(const Binary& expr) override;

	std::any visitCommaExpr(const Comma& expr) override;

	std::any visitTernaryExpr(const Ternary& expr) override;

	std::any visitExpressionStmt(const Expression& stmt) override;

	std::any visitPrintStmt(const Print& stmt) override;

private:

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