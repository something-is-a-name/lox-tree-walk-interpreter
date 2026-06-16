#pragma once
#include <string>

class Lox {
public:
     void runFile( const std::string& path);
     void runPrompt();

    static void error(int line, const std::string& message);

    static void error(Token& token, const std::string& message);

private:
     void run( std::string& source);

    static void report(
        int line,
        std::string& where,
        std::string& message
    );

    inline static bool hadError{ false };
};
