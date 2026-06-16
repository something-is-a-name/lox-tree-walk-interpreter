#include "Lox.h"
#include <iostream>
#include <stdexcept>


int main(int argc, char* argv[]) {

	try {
		if (argc > 2) {
			std::cerr << "Usage: cpplox [script]\n";
			return 64;
		}

		if (argc == 2) {
			Lox::runFile(argv[1]);
		}
		else {
			Lox::runPrompt();
		}
	}
	catch (const std::runtime_error&) {
		return 1;
	}

	return 0;
}