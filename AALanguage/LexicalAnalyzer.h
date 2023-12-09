#pragma once

#include "Trie.h"
#include <regex>
#include <string>

struct Token;

class LexicalAnalyzer {
public:
	enum literal_type {
		logical, integer, real, symbol, string, none
	};
	enum token_type {
		service, identifier, literal, operation, punctuation, comma, brace, comment, unknown
	};

	bool is_service(Trie* service_trie, std::string s);
	bool is_identifier(std::string s);
	literal_type is_literal(std::string s);
	bool is_operation(std::string s);
	bool is_punctuation(std::string s);
	bool is_comma(std::string s);
	bool is_brace(std::string s);
	bool is_comment(std::string s);
	token_type get_token_type(Trie* service_trie, std::string s);
	void print_token(Trie* service_trie, std::string buffer);

	Token get_token();

	LexicalAnalyzer(std::string path);
private:
	int line_ = 1;
	int curr_ = 0;
	std::vector<Token> tokens_;
};

struct Token {
	LexicalAnalyzer::token_type type;
	std::string value;
	int line;
	Token(LexicalAnalyzer::token_type type_, std::string value_, int line_) {
		type = type_;
		value = value_;
		line = line_;
	}
	Token() {
		type = LexicalAnalyzer::token_type::unknown;
		value = "";
		line = -1;
	}
};