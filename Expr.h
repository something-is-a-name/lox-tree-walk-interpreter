#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Token.h"

class Binary;
class Expr;
class Grouping;
class Literal;
class Unary;
class Comma;
class Ternary;
class Variable;

class ExprVisitor {
public:
    virtual std::any visitBinaryExpr(const Binary& expr) = 0;
    virtual std::any visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::any visitLiteralExpr(const Literal& expr) = 0;
    virtual std::any visitUnaryExpr(const Unary& expr) = 0;
    virtual std::any visitCommaExpr(const Comma& expr) = 0;
    virtual std::any visitTernaryExpr(const Ternary& expr) = 0;
    virtual std::any visitVariableExpr(const Variable& expr) = 0;

    virtual ~ExprVisitor() = default;
};

class Expr {
public:
    virtual ~Expr() = 0;
    virtual std::any accept(ExprVisitor& visitor) const = 0;
};

class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> left,
        Token op,
        std::unique_ptr<Expr> right);

    std::any accept(ExprVisitor& visitor)const  override;

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    explicit Grouping(std::unique_ptr<Expr> expression);

    std::any accept(ExprVisitor& visitor)const  override;

    std::unique_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    explicit Literal(std::any value);

    std::any accept(ExprVisitor& visitor)const  override;

    std::any value;
};

class Unary : public Expr {
public:
    Unary(Token op, std::unique_ptr<Expr> right);

    std::any accept(ExprVisitor& visitor)const  override;

    Token op;
    std::unique_ptr<Expr> right;
};

class Comma : public Expr {
public:
    Comma(std::vector<std::unique_ptr<Expr>> exprs);

    std::any accept(ExprVisitor& visitor)const  override;
    std::vector<std::unique_ptr<Expr>> exprs;
};

class Ternary : public Expr {
public:
    Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenExpr, std::unique_ptr<Expr> elseExpr);

    std::any accept(ExprVisitor& visitor)const  override;
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> thenExpr;
    std::unique_ptr<Expr> elseExpr;
};

class Variable : public Expr {
public:
    Variable(Token name);

    std::any accept(ExprVisitor& visitor)const override;

    Token name;
};