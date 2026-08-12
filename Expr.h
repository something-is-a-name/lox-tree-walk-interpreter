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
class Assign;
class Logical;
class Call;
class Get;
class Set;
class This;
class Super;
class Array;
class Index;

class ExprVisitor {
public:
    virtual std::any visitBinaryExpr(const Binary& expr) = 0;
    virtual std::any visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::any visitLiteralExpr(const Literal& expr) = 0;
    virtual std::any visitUnaryExpr(const Unary& expr) = 0;
    virtual std::any visitCommaExpr(const Comma& expr) = 0;
    virtual std::any visitTernaryExpr(const Ternary& expr) = 0;
    virtual std::any visitVariableExpr(const Variable& expr) = 0;
    virtual std::any visitAssignExpr(const Assign& expr) = 0;
    virtual std::any visitLogicalExpr(const Logical& expr) = 0;
    virtual std::any visitCallExpr(const Call& expr) = 0;
    virtual std::any visitGetExpr(const Get& expr) = 0;
    virtual std::any visitSetExpr(const Set& expr) = 0;
    virtual std::any visitThisExpr(const This& expr) = 0;
    virtual std::any visitSuperExpr(const Super& expr) = 0;
    virtual std::any visitArrayExpr(const Array& expr) = 0;
    virtual std::any visitIndexExpr(const Index& expr) = 0;

    virtual ~ExprVisitor() = default;
};

class Expr {
public:
    virtual ~Expr();
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

class Assign : public Expr {
public:
    Assign(Token name, std::unique_ptr<Expr> value);

    std::any accept(ExprVisitor& visitor)const override;

    Token name;
    std::unique_ptr<Expr> value;
};

class Logical : public Expr {
public:
    Logical(std::unique_ptr<Expr> left,
        Token op,
        std::unique_ptr<Expr> right);

    std::any accept(ExprVisitor& visitor)const override;

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

class Call : public Expr {
public:
    Call(std::unique_ptr<Expr> callee,
        Token paren,
        std::vector<std::unique_ptr<Expr>> arguments);

    std::any accept(ExprVisitor& visitor)const override;

    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;
};

class Get : public Expr {
public:
    Get(std::unique_ptr<Expr> object, Token name);

    std::any accept(ExprVisitor& visitor)const override;

    std::unique_ptr<Expr> object;
    Token name;
};

class Set : public Expr {
public:
    Set(std::unique_ptr<Expr> object, Token name, std::unique_ptr<Expr> value);

    std::any accept(ExprVisitor& visitor)const override;

    std::unique_ptr<Expr> object;
    Token name;
    std::unique_ptr<Expr> value;
};

class This : public Expr {
public:
    This(Token keyword);

    std::any accept(ExprVisitor& visitor)const  override;

    Token keyword;
};

class Super : public Expr {
public:
    Super(Token keyword, Token method);

    std::any accept(ExprVisitor& visitor)const override;

    Token keyword;
    Token method;
};

class Array : public Expr {
public:
    Array(std::vector<std::unique_ptr<Expr>> elems);

    std::any accept(ExprVisitor& viistor)const  override;
    std::vector<std::unique_ptr<Expr>> elems;
};

class Index : public Expr {
public:
    Index(Token token, std::unique_ptr<Expr> arr, std::unique_ptr<Expr> index);

    std::any accept(ExprVisitor& visitor)const  override;

    Token token;
    std::unique_ptr<Expr> arr;
    std::unique_ptr<Expr> index;
};