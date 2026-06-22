#pragma once


#include "Lox.h"
#include "Interpreter.h"

Interpreter Lox::interpreter = Interpreter();

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

 void Lox::run(const std::string& source) {
     Scanner scanner = Scanner(source);
     std::vector<Token> tokens = scanner.scanTokens();
     Parser parser = Parser(tokens);
     std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

     if (hadError) return;

     interpreter.interpret(statements);

 }


 void Lox::report(int line, const std::string& where, const std::string& message) {
     std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
     hadError = true;
 }
