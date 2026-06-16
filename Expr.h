#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Token.h"

class Binary;
class Grouping;
class Literal;
class Unary;
class Comma;

class ExprVisitor {
public:
    virtual std::string visitBinaryExpr(const Binary& expr) = 0;
    virtual std::string visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::string visitLiteralExpr(const Literal& expr) = 0;
    virtual std::string visitUnaryExpr(const Unary& expr) = 0;
    virtual std::string visitCommaExpr(const Comma& expr) = 0;
    virtual std::string visitTernaryExpr(const Ternary& expr) = 0;

    virtual ~ExprVisitor() = default;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string accept(ExprVisitor& visitor) const = 0;
};

class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> left,
        Token op,
        std::unique_ptr<Expr> right)
        : left(std::move(left)),
        op(std::move(op)),
        right(std::move(right)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    explicit Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitGroupingExpr(*this);
    }

    std::unique_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    explicit Literal(std::any value)
        : value(std::move(value)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }

    std::any value;
};

class Unary : public Expr {
public:
    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)),
        right(std::move(right)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }

    Token op;
    std::unique_ptr<Expr> right;
};

class Comma : public Expr {
public:
    Comma(std::vector<std::unique_ptr<Expr>> exprs) :
        exprs(std::move(exprs)) {}

    std::string accept(ExprVisitor& visitor) const override;
    std::vector<std::unique_ptr<Expr>> exprs;
};

class Ternary : public Expr {
public:
    Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenExpr, std::unique_ptr<Expr> elseExpr) :
        condition{ std::move(condition) }, thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}

    std::string accept(ExprVisitor& visitor) const override;
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> thenExpr;
    std::unique_ptr<Expr> elseExpr;
};