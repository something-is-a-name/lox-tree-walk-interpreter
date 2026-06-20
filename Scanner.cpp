#include "Scanner.h"
#include "Lox.h"

bool Scanner::isAtEnd() {
	return current >= source.length();
}

char Scanner::advance() {
	return source.at(current++);
}

void Scanner::addToken(TokenType type, std::any literal) {
	std::string text {source.substr(start, current - start)};
	Token newToken{ type, text, literal, line };
	tokens.push_back(newToken);
}

void Scanner::addToken(TokenType type) {
	addToken(type, std::any{});
}

bool Scanner::match(char expected) {
	if (isAtEnd()) return false;
	if (source.at(current) != expected) return false;

	current++;
	return true;
}

char Scanner::peek() { // Looks ahead at the next character
	if (isAtEnd()) return '\0';
	return source.at(current);
}

char Scanner::peekNext() {
	if (current + 1 >= source.length()) return '\0';
	return source.at(current + 1);
}

void Scanner::string() {
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

bool Scanner::isDigit(char c) {
	return c >= '0' && c <= '9';
}

void Scanner::number() {
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

bool Scanner::isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '_');
}

bool Scanner::isAlphaNumeric(char c) {
	return isAlpha(c) || isDigit(c);
}

void Scanner::identifier() {
	while (isAlphaNumeric(peek())) advance();

	std::string text { source.substr(start, current - start)};
	TokenType type{};
	if (keywords.contains(text)) type = keywords.at(text);
	else type = IDENTIFIER;

	addToken(type);
}


void Scanner::scanToken() {
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

Scanner::Scanner(std::string source) :
	source{ source }
{}

std::vector<Token> Scanner::scanTokens() {
	while (!isAtEnd()) { // Go to beginnning of next lexeme
		start = current;
		scanToken();
	}

	Token newToken{ EoF,"", std::any{}, line };
	tokens.push_back(newToken);
	return tokens;
}