#include "Scanner.h"
#include "Lox.h"

class Scanner {
private:
	const std::string source;
	std::vector<Token> tokens {};

	int start{ 0 };
	int current{ 0 };
	int line{ 1 };

	const std::map<std::string, TokenType> keywords {
		{"and", AND},
		{ "class", CLASS },
		{ "else", ELSE },
		{ "false", FALSE },
		{ "for", FOR },
		{ "fun", FUN },
		{ "if", IF },
		{ "nil", NIL },
		{ "or", OR },
		{ "print", PRINT },
		{ "return", RETURN },
		{ "super", SUPER },
		{ "this", THIS },
		{ "true", TRUE },
		{ "var", VAR },
		{ "while", WHILE }
	};


	bool isAtEnd() {
		return current >= source.length();
	}

	char advance() {
		return source.at(current++);
	}

	void addToken(TokenType type, std::any literal) {
		std::string text {source.substr(start, current - start)};
		Token newToken{ type, text, literal, line };
		tokens.push_back(newToken);
	}

	void addToken(TokenType type) {
		addToken(type, std::any{});
	}

	bool match(char expected) {
		if (isAtEnd()) return false;
		if (source.at(current) != expected) return false;

		current++;
		return true;
	}

	char peek() { // Looks ahead at the next character
		if (isAtEnd()) return '\0';
		return source.at(current);
	}

	char peekNext() {
		if (current + 1 >= source.length()) return '\0';
		return source.at(current + 1);
	}

	void string() {
		while (peek() != '"' && !isAtEnd()) {
			if (peek() == '\n') line++; // Allow multi-line strings
			advance();
		}

		if (isAtEnd()) {
			std::string errorStr { "unterminated string"};
			Lox::error(line, errorStr);
			return;
		}

		// The closing ""
		advance();

		// Trim the surrounding quotes
		std::string value { source.substr(start + 1, current - start - 2)};
		addToken(STRING, value);

	}

	bool isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	void number() {
		while (isDigit(peek())) advance();

		// Look for fractional part
		if (peek() == '.' && isDigit(peekNext())) {
			// Consume the .
			advance();

			while (isDigit(peek())) advance();
		}

		std::string numStr = source.substr(start, current - start);
		addToken(NUMBER, std::stod(numStr));
	}

	bool isAlpha(char c) {
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c == '_');
	}

	bool isAlphaNumeric(char c) {
		return isAlpha(c) || isDigit(c);
	}

	void identifier() {
		while (isAlphaNumeric(peek())) advance();

		std::string text { source.substr(start, current - start)};
		TokenType type{};
		if (keywords.contains(text)) type = keywords.at(text);
		else type = IDENTIFIER;

		addToken(type);
	}


	void scanToken() {
		char c = advance();
		switch (c) {
		case '(': addToken(LEFT_PAREN); break;
		case ')': addToken(RIGHT_PAREN); break;
		case '{': addToken(LEFT_BRACE); break;
		case '}': addToken(RIGHT_BRACE); break;
		case ',': addToken(COMMA); break;
		case '.': addToken(DOT); break;
		case '-': addToken(MINUS); break;
		case '+': addToken(PLUS); break;
		case ';': addToken(SEMICOLON); break;
		case ':': addToken(COLON); break;
		case '*': addToken(STAR); break;
		case '?': addToken(QUESTION); break;

		case '!':
			addToken(match('=') ? BANG_EQUAL : BANG);
			break;

		case '=':
			addToken(match('=') ? EQUAL_EQUAL : EQUAL);
			break;

		case '<':
			addToken(match('=') ? LESS_EQUAL : LESS);
			break;

		case '>':
			addToken(match('=') ? GREATER_EQUAL : GREATER);
			break;

		case '/':
			if (match('/')) { // A single-line comment
				while (peek() != '\n' && !isAtEnd()) advance();
			}

			else if (match('*')) { // A multi-line comment
				while (!isAtEnd()) {
					if (peek() == '\n') line++;

					if (peek() == '*' && peekNext() == '/') {
						advance();
						advance();
						break;
					}

					advance();
				}
			}

			else {
				addToken(SLASH);
			}
			break;

			// Ignore whitespace
		case ' ':
		case '\r':
		case '\t':
			break;

		case '\n':
			line++;
			break;

		case '"': string(); break;


		default:
			if (isDigit(c)) {
				number();
			}
			else if (isAlpha(c)) {
				identifier();
			}
			else {
				std::string errorStr = "unexpected character";
				Lox::error(line, errorStr);
			}
			break;

		}
	}

public:
	Scanner(std::string source) :
		source{ source }
	{}

	std::vector<Token> scanTokens() {
		while (!isAtEnd()) { // Go to beginnning of next lexeme
			start = current;
			scanToken();
		}

		Token newToken{ EoF,"", std::any{}, line };
		tokens.push_back(newToken);
		return tokens;
	}
};