#include "Expr.h"


// Expr

Expr::~Expr() = default;

// Binary

Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
	left(std::move(left)), right(std::move(right)), op(std::move(op))
{} 

std::any Binary::accept(ExprVisitor& visitor) const {
	return visitor.visitBinaryExpr(*this);
}

// Grouping

Grouping::Grouping(std::unique_ptr<Expr> expression)
	: expression(std::move(expression))
{}

std::any Grouping::accept(ExprVisitor& visitor) const {
	return visitor.visitGroupingExpr(*this);
}

// Literal

Literal::Literal(std::any value) :
	value(std::move(value)) {}

std::any Literal::accept(ExprVisitor& visitor) const {
	return visitor.visitLiteralExpr(*this);
}

// Unary

Unary::Unary(Token op, std::unique_ptr<Expr> right) :
	op(std::move(op)), right(std::move(right)) {}

std::any Unary::accept(ExprVisitor& visitor) const {
	return visitor.visitUnaryExpr(*this);
}

// Comma

Comma::Comma(std::vector<std::unique_ptr<Expr>> exprs) :
	exprs(std::move(exprs)) {}

std::any Comma::accept(ExprVisitor& visitor) const {
	return visitor.visitCommaExpr(*this);
}

// Ternary 

Ternary::Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenExpr, std::unique_ptr<Expr> elseExpr) :
	condition{ std::move(condition) }, thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}

std::any Ternary::accept(ExprVisitor& visitor) const {
	return visitor.visitTernaryExpr(*this);
}
