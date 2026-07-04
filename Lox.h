#pragma once
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <string>
#include "Token.h"
#include "RuntimeError.h"

#include "Scanner.h"
#include "Parser.h"

class Interpreter;

class Lox {
public:
     static void runFile( const std::string& path);
     static void runPrompt();

     static void error(int line, const std::string& message);

     static void error(const Token& token, const std::string& message);

     static void runtimeError(RuntimeError error);

private:
    static void run( const std::string& source);

    static void report(
        int line,
        const std::string& where,
        const std::string& message
    );

    inline static bool hadError{ false };
    inline static bool hadRuntimeError{ true };

     static Interpreter interpreter;

     inline static std::vector<std::unique_ptr<Stmt>> allStatements;

};
