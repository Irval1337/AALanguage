#pragma once

#include "Trie.h"
#include <regex>
#include <string>
#include "LibParser.h"

struct Token;

//! A class that allows you to break program source code into indivisible grammar tokens.
class LexicalAnalyzer {
public:
	//! Enumeration for a literal category
	enum literal_type {
		logical, integer, real, symbol, string, none
	};
	//! Enumeration for possible types of language tokens. Tokens with the comment type are ignored
	enum token_type {
		service, identifier, literal, operation, punctuation, comma, brace, comment, unknown
	};
	/**
	* @brief Getting the next token from the generated array.
	*
	* @return Token class instance for the following lexeme.
	*/
	Token get_token();
	/**
	* @brief Getting the previous token from the generated array.
	*
	* @return Token class instance for the previous lexeme.
	*/
	Token prev_token();
	/**
	* @brief Gains information about the success of lexical analysis.
	*
	* @return True if the analysis was error-free - no unexpected character set was encountered that is not a token of the language.
	*/
	bool get_ok() const;
	/**
	* @brief Checks if the string is a valid literal from the language grammar.
	* 
	* Performs a check for each literal type using std::regex_match.
	*
	* @param s String to validate
	* 
	* @return The appropriate literal enumerator, or none if the string is not a literal.
	*/
	literal_type is_literal(std::string s);
	/**
	* @brief Getting all tokens from the analyzed file.
	*
	* @return Vector array with all tokens obtained during lexical analysis.
	*/
	std::vector<Token> get_tokens();

	/**
	* @brief A class constructor that performs lexical analysis on the specified file.
	* 
	* Uses Trie to create a dictionary with all the reserved words of a language.
	*
	* Greedily types characters into the current token until an error occurs due to an unexpected character. Amortized asymptotics of O(n) partitioning.
	*
	* Causes an exception if parsing a token with type unknown.
	*
	* Replaces all using directives with an array of tokens of libraries used in these directives.
	*
	* @param path Path to the file with the source code of the program in AALanguage
	* @param lib_parser Reference to an instance of the LibParser class for parsing code from built-in libraries
	* 
	* @return Vector array with all tokens obtained during lexical analysis.
	*/
	LexicalAnalyzer(std::string path, LibParser& lib_parser);
private:
	int line_ = 1;
	int curr_ = 0;
	bool ok_ = true;
	std::vector<Token> tokens_;

	bool is_service(Trie* service_trie, std::string s);
	bool is_identifier(std::string s);
	bool is_operation(std::string s);
	bool is_punctuation(std::string s);
	bool is_comma(std::string s);
	bool is_brace(std::string s);
	bool is_comment(std::string s);
	token_type get_token_type(Trie* service_trie, std::string s);
	void print_token(Trie* service_trie, std::string buffer);
	std::vector<Token> merge(std::vector<Token>& f, std::vector<Token>& s, int& ind, int);
};

//! A stuct used to store information about a language token.
struct Token {
	//! Internal token type
	LexicalAnalyzer::token_type type;
	//! Token string equivalent
	std::string value;
	//! The number of the line containing this token. The global counter for the number of occurrences of '\n' character is used for counting
	int line;
	/**
	* @brief A constructor that specifies the public fields of the token.
	*/
	Token(LexicalAnalyzer::token_type type_, std::string value_, int line_) {
		type = type_;
		value = value_;
		line = line_;
	}
	/**
	* @brief A default constructor, that sets:
	* - type = Unknown
	* - value = ""
	* - line = -1
	*/
	Token() {
		type = LexicalAnalyzer::token_type::unknown;
		value = "";
		line = -1;
	}
};