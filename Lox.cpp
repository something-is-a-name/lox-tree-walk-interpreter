#pragma once


#include "Interpreter.h"
#include "Resolver.h"
#include "Scanner.h"
#include "Parser.h"
#include "Stmt.h"


Interpreter Lox::interpreter = Interpreter();
std::vector<std::unique_ptr<Stmt>> Lox::allStatements{};

void Lox::runFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);

    std::string source(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    if (hadError)
        throw std::runtime_error("Lox error");

}

void Lox::runPrompt() {
    std::string line;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line))
            break;

        run(line);
        hadError = false;
    }
}

 void Lox::error(int line, const std::string& message) {
    report(line, "", message);
}

 void Lox::error(const Token& token, const std::string& message) {
     if (token.type == TokenType::EoF)
         report(token.line, " at end", message);
     else
         report(token.line, " at '" + token.lexeme + "'", message);
 }

 void Lox::runtimeError(RuntimeError error) {
     std::cerr << error.what() << "\n [line " << error.token.line << "]";
     hadRuntimeError = true;
 }

 void Lox::run(const std::string& source)
 {
     Scanner scanner(source);
     auto tokens = scanner.scanTokens();

     Parser parser(tokens);
     auto statements = parser.parse();

     if (hadError) return;

     Resolver resolver(interpreter);
     resolver.resolve(statements);

     if (hadError) return;


     for (auto& stmt : statements)
         allStatements.push_back(std::move(stmt));


     interpreter.interpret(allStatements);
 }


 void Lox::report(int line, const std::string& where, const std::string& message) {
     std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
     hadError = true;
 }
