#include <regex>
#include <iostream>
#include "LexicalAnalyzer.h"

int main() {
	LexicalAnalyzer lex("C:\\1\\1.txt");
	Token token;
	while (token = lex.get_token(), token.line != -1) {
		std::cout << token.value << "\n";
	}
}