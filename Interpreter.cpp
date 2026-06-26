#include "Interpreter.h"
#include "Lox.h"



void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
	try {
		for (auto& statement : statements) {
			execute(*statement);
		}
	}
	catch (RuntimeError error) {
		Lox::runtimeError(error);
	}
}

std::any Interpreter::visitLiteralExpr(const Literal& expr)  {
	return expr.value;
}

std::any Interpreter::visitGroupingExpr(const Grouping& expr)  {
	return evaluate(*expr.expression);
}

std::any Interpreter::visitUnaryExpr(const Unary& expr)  {
	auto right = evaluate(*expr.right);

	switch (expr.op.type) {
	case BANG:
		return !isTruthy(right);

	case MINUS:
		checkNumberOperand(expr.op, right);
		return -asNumber(right);
	}

	return nullptr;
}

std::any Interpreter::visitBinaryExpr(const Binary& expr)  {
	auto left = evaluate(*expr.left);
	auto right = evaluate(*expr.right);

	switch (expr.op.type) {
	case BANG_EQUAL:
		return  !isEqual(left, right);
	case EQUAL_EQUAL:
		return  isEqual(left, right);
	case GREATER:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) > asNumber(right);
	case GREATER_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) >= asNumber(right);
	case LESS:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) < asNumber(right);
	case LESS_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) <= asNumber(right);
	case MINUS:
		checkNumberOperands(expr.op, left, right);
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) - asNumber(right);
	case SLASH:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) / asNumber(right);
	case STAR:
		checkNumberOperands(expr.op, left, right);
		return asNumber(left) * asNumber(right);
	case PLUS:
		if (left.type() == typeid(double) && right.type() == typeid(double)) {
			return asNumber(left) + asNumber(right);
		}

		if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
			return asString(left) + asString(right);
		}
		else {
			throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
		}
	}


	return nullptr;
}

std::any Interpreter::visitCommaExpr(const Comma& expr)  {
	std::any result;

	for (auto& e : expr.exprs) {
		result = evaluate(*e);
	}

	return result;
}

std::any Interpreter::visitTernaryExpr(const Ternary& expr) {

	if (expr.condition) {
		return evaluate(*expr.thenExpr);
	}
	else {
		return evaluate(*expr.elseExpr);
	}
}

std::any Interpreter::visitVariableExpr(const Variable& expr) {
	return environment.get(expr.name);
}


std::any Interpreter::evaluate(const Expr& expr) {
	return expr.accept(*this);
}

void Interpreter::execute(const Stmt& stmt) {
	stmt.accept(*this);
}

std::any Interpreter::visitExpressionStmt(const Expression& stmt) {
	evaluate(*stmt.expression);

	return nullptr;
}

std::any Interpreter::visitPrintStmt(const Print& stmt) {
	auto value = evaluate(*stmt.expression);
	std::cout << stringify(value);
	
	return nullptr;
}

std::any Interpreter::visitVarStmt(const Var& stmt) {
	std::any value {};
	if (stmt.initializer != nullptr) {
		value = evaluate(*stmt.initializer);
	}
	// Set to empty value if no initializer is given (nil)
	environment.define(stmt.name.lexeme, value);
	return nullptr;
}


bool  Interpreter::isTruthy(std::any& v) {
	if (!v.has_value()) return false;
	if (v.type() == typeid(bool)) return asBool(v);
	return true;
}

bool Interpreter::isEqual(std::any& a, std::any& b) {
	if (!a.has_value() && !b.has_value()) return true;
	if (!a.has_value()) return false;

	if (a.type() != b.type()) return false;

	if (auto av = std::any_cast<double>(&a)) {
		return *av == std::any_cast<double>(b);
	}

	if (auto av = std::any_cast<bool>(&a)) {
		return *av == std::any_cast<bool>(b);
	}

	if (auto av = std::any_cast<std::string>(&a)) {
		return *av == std::any_cast<std::string>(b);
	}

	return false;
}

double Interpreter::asNumber(const std::any& v) {
	return std::any_cast<double>(v);
}

bool Interpreter::asBool(const std::any& v) {
	return std::any_cast<bool>(v);
}

std::string Interpreter::asString(const std::any& v) {
	return std::any_cast<std::string>(v);
}

void Interpreter::checkNumberOperand(Token op, std::any& operand) {
	if (operand.type() == typeid(double)) return;
	throw RuntimeError(op, " Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, std::any& left, std::any& right) {
	if (left.type() == typeid(double) && right.type() == typeid(double)) return;
	throw RuntimeError(op, " Operands must be numbers.");
}

std::string Interpreter::stringify(const std::any& v) {
	if (!v.has_value()) return "nil";

	if (v.type() == typeid(double)) {
		double x = asNumber(v);

		std::ostringstream out {};
		out << x;
		return out.str();
	}

	if (v.type() == typeid(bool)) {
		if (asBool(v)) { return "true"; }
		else { return "false"; }
	}

	else {
		return asString(v);

	}
}
