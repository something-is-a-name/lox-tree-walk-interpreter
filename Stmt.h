#pragma once
#include <any>
#include <vector>
#include <memory>
#include "Expr.h"

class Stmt;
class Block;
class Expression;
class Print;
class Var;


class StmtVisitor {
public:
    //virtual std::any visitBlockStmt(const Block& stmt) = 0;
    virtual std::any visitExpressionStmt(const Expression& stmt) = 0;
    virtual std::any visitPrintStmt(const Print& stmt) = 0;
    //virtual std::any visitVarStmt(const Var& stmt) = 0;

    virtual ~StmtVisitor() = default;
};


class Stmt {
public:
    virtual ~Stmt() = 0;
    virtual std::any accept(StmtVisitor& visitor) const = 0;
};


class Block : public Stmt {
public:
    Block(std::vector<std::unique_ptr<Stmt>> statements);

    std::any accept(StmtVisitor& visitor) const  override;

    std::vector<std::unique_ptr<Stmt>> statements;

};

class Expression : public Stmt {
public:
    Expression(std::unique_ptr<Expr> expression);

    std::any accept(StmtVisitor& visitor)const   override;

    std::unique_ptr<Expr> expression;

};

class Print : public Stmt {
public:
    Print(std::unique_ptr<Expr> expression);

    std::any accept(StmtVisitor& visitor)const   override;

    std::unique_ptr<Expr> expression;
};

class Var : public Stmt {
public:
    Var(Token name, std::unique_ptr<Expr> initializer);

    std::any accept(StmtVisitor& visitor)const   override;

    Token name;
    std::unique_ptr<Expr> initializer;
};