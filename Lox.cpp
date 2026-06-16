#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "Token.h"

class Lox {
public:
    void runFile(const std::string& path) {
        std::ifstream input(path, std::ios::binary);

        std::string source(
            (std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()
        );

        run(source);

        if (hadError)
            throw std::runtime_error("Lox error");
    }

    void runPrompt() {
        std::string line;

        while (true) {
            std::cout << "> ";

            if (!std::getline(std::cin, line))
                break;

            run(line);
            hadError = false;
        }
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }

    static void error(const Token &token, const std::string& message) {
        if (token.type == TokenType::EoF)
            report(token.line, " at end", message);
        else
            report(token.line, " at '" + token.lexeme + "'", message);
    }

private:
    void run(const std::string& source) {
        std::cout << source << '\n';
    }

    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
        hadError = true;
    }

    inline static bool hadError { false };
};