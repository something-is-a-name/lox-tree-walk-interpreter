#include "Resolver.h"
#include "Interpreter.h"
#include "Lox.h"

Resolver::Resolver(Interpreter& interpreter) :
	interpreter (interpreter)
{}

std::any Resolver::visitBlockStmt(const Block& stmt)
{
	beginScope();
	resolve(stmt.statements);
	endScope();
	return nullptr;
}

std::any Resolver::visitVarStmt(const Var& stmt)
{
	declare(stmt.name);
	
	if (stmt.initializer != nullptr) {
		resolve(*stmt.initializer);
	}

	define(stmt.name);

	return nullptr;
}

std::any Resolver::visitFunctionStmt(const Function& stmt)
{
	declare(stmt.name);
	define(stmt.name);

	resolveFunction(stmt, FunctionType::FUNCTION);

	return nullptr;
}

std::any Resolver::visitReturnStmt(const ReturnStmt& stmt)
{
	if (currentFunction == FunctionType::NONE) {
		Lox::error(stmt.keyword, "Can't return from top level code");
	}

	if (stmt.value != nullptr) {
		resolve(*stmt.value);
	}
	return nullptr;
}

std::any Resolver::visitWhileStmt(const While& stmt)
{
	resolve(*stmt.condition);
	resolve(*stmt.body);
	return nullptr;
}

std::any Resolver::visitIfStmt(const If& stmt)
{
	resolve(*stmt.condition);
	resolve(*stmt.thenBranch);

	if (stmt.elseBranch != nullptr) resolve(*stmt.elseBranch);
	return nullptr;
}

std::any Resolver::visitPrintStmt(const Print& stmt)
{
	resolve(*stmt.expression);
	return nullptr;
}

std::any Resolver::visitExpressionStmt(const Expression& stmt)
{
	resolve(*stmt.expression);
	return nullptr;
}

std::any Resolver::visitClassStmt(const Class& stmt)
{
	declare(stmt.name);
	define(stmt.name);
	return nullptr;
}

std::any Resolver::visitVariableExpr(const Variable& expr)
{
	if (!scopes.empty() &&
		scopes.back().contains(expr.name.lexeme) &&
		!scopes.back().at(expr.name.lexeme)) {
		Lox::error(expr.name, "Can't read local variable in its own initializer.");
	}

	resolveLocal(expr, expr.name);
	return nullptr;
}

std::any Resolver::visitAssignExpr(const Assign& expr)
{
	resolve(*expr.value);
	resolveLocal(expr, expr.name);

	return nullptr;
}

std::any Resolver::visitBinaryExpr(const Binary& expr)
{
	resolve(*expr.left);
	resolve(*expr.right);
	return nullptr;
}

std::any Resolver::visitCallExpr(const Call& expr)
{
	resolve(*expr.callee);

	for (auto& argument : expr.arguments) {
		resolve(*argument);
	}
	return nullptr;
}

std::any Resolver::visitGroupingExpr(const Grouping& expr)
{
	resolve(*expr.expression);
	return nullptr;
}

std::any Resolver::visitLiteralExpr(const Literal& expr)
{
	return nullptr;
}

std::any Resolver::visitLogicalExpr(const Logical& expr)
{
	resolve(*expr.left);
	resolve(*expr.right);

	return nullptr;
}

std::any Resolver::visitUnaryExpr(const Unary& expr)
{
	resolve(*expr.right);

	return nullptr;
}

std::any Resolver::visitCommaExpr(const Comma& expr)
{
	resolve(expr.exprs);
	return nullptr;
}

std::any Resolver::visitTernaryExpr(const Ternary& expr)
{
	resolve(*expr.condition);
	resolve(*expr.thenExpr);
	resolve(*expr.elseExpr);
	return nullptr;
}

std::any Resolver::visitGetExpr(const Get& expr)
{
	resolve(*expr.object);
	return nullptr;
}

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements)
{
	for (const auto& statement : statements) {
		resolve(*statement);
	}
}

void Resolver::resolve(const std::vector<std::unique_ptr<Expr>>& statements)
{
	for (const auto& statement : statements) {
		resolve(*statement);
	}
}

void Resolver::resolve(const Stmt& stmt)
{
	stmt.accept(*this);
}

void Resolver::resolve(const Expr& expr)
{
	expr.accept(*this);
}

void Resolver::beginScope()
{
	scopes.emplace_back(std::map<std::string, bool>());
}

void Resolver::endScope()
{
	scopes.pop_back();
}

void Resolver::declare(Token name)
{
	if (scopes.empty()) return;

	std::map<std::string, bool> scope = scopes.back();

	if (scope.contains(name.lexeme)) {
		Lox::error(name, "Already a variable with this name is this scope.");
	}

	scope.emplace(name.lexeme, false);
}

void Resolver::define(Token name)
{
	if (scopes.empty()) return;

	scopes.back().emplace(name.lexeme, true);
}

void Resolver::resolveLocal(const Expr& expr, Token name)
{
	for (int i = scopes.size() - 1; i >= 0; i--) {
		if (scopes.at(i).contains(name.lexeme)) {
			interpreter.resolve(expr, scopes.size() - 1 - i);
		}
	}
}

void Resolver::resolveFunction(const Function& function, FunctionType type)
{
	FunctionType enclosingFunction = currentFunction;
	currentFunction = type;

	beginScope();
	for (Token param : function.params) {
		declare(param);
		define(param);	
	}
	resolve(function.body);
	endScope();

	currentFunction = enclosingFunction;
}


