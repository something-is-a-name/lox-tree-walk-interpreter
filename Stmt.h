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
class If;
class While;
class Function;
class ReturnStmt;
class Class;


class StmtVisitor {
public:
    virtual std::any visitBlockStmt(const Block& stmt) = 0;
    virtual std::any visitExpressionStmt(const Expression& stmt) = 0;
    virtual std::any visitPrintStmt(const Print& stmt) = 0;
    virtual std::any visitVarStmt(const Var& stmt) = 0;
    virtual std::any visitIfStmt(const If& stmt) = 0;
    virtual std::any visitWhileStmt(const While& stmt) = 0;
    virtual std::any visitFunctionStmt(const Function& stmt) = 0;
    virtual std::any visitReturnStmt(const ReturnStmt& stmt) = 0;
    virtual std::any visitClassStmt(const Class& stmt) = 0;

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

class If : public Stmt {
public:
    If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);

    std::any accept(StmtVisitor& visitor)const  override;

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

class While : public Stmt {
public:
    While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);

    std::any accept(StmtVisitor& visitor)const override;

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};


class Function : public Stmt {
public:
    Function(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body);

    std::any accept(StmtVisitor& visitor) const  override;

    Token name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;

};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(Token keyword, std::unique_ptr<Expr> value);

    std::any accept(StmtVisitor& visitor)const   override;

    Token keyword;
    std::unique_ptr<Expr> value;
};

class Class : public Stmt {
public:
    Class(Token name, std::vector<std::unique_ptr<Function>> methods);

    std::any accept(StmtVisitor& visitor)const  override;

    Token name;
    //std::unique_ptr<Variable> superclass;
    std::vector<std::unique_ptr<Function>> methods;
};

