#include <regex>
#include <iostream>
#include "Trie.h"
#include <fstream>
#include <regex>

enum literal_type {
	logical, integer, real, symbol, string, none
};

enum token_type {
	service, identifier, literal, operation, punctuation, comma, brace, comment, unknown
};

bool is_service(Trie* service_trie, std::string s) {
	return service_trie->find(s.begin(), s.end()) != nullptr;
}

bool is_identifier(std::string s) {
	static const std::regex r(R"([a-zA-Z_][\w]*)");
	return std::regex_match(s, r);
}

literal_type is_literal(std::string s) {
	if (s == "true" || s == "false") return literal_type::logical;

	static const std::regex integer_r(R"([0-9]{1,}(UL|ul|L|l|I|i|U|u|s|S)?)");
	if (std::regex_match(s, integer_r)) return literal_type::integer;

	static const std::regex real_r(R"(([0-9]{1,}.[0-9]*)(e(\+|-)?[0-9]{1,})?(d|D|f|F)?)");
	if (std::regex_match(s, real_r)) return literal_type::real;

	static const std::regex symbol_r(R"(\'(.|\\n|\\t|\\0)\')");
	if (std::regex_match(s, symbol_r)) return literal_type::symbol;

	static const std::regex string_r(R"(".*")");
	if (std::regex_match(s, string_r)) return literal_type::string;

	return literal_type::none;
}

bool is_operation(std::string s) {
	return s == "[" || s == "]" || s == "." || s == "++" || s == "--" || s == "+" || s == "-" || s == "!" || s == "~" || s == "*" || s == "/" || s == "%" ||
		s == ">>" || s == "<<" || s == ">" || s == ">=" || s == "<" || s == "<=" || s == "==" || s == "!=" || s == "&" || s == "^" || s == "|" ||
		s == "&&" || s == "**" || s == "||" || s == "=" || s == "+=" || s == "-=" || s == "*=" || s == "/=" || s == "%=";
}

bool is_punctuation(std::string s) {
	return s == ":" || s == ";" || s == "{" || s == "}";
}

bool is_comma(std::string s) {
	return s == ",";
}

bool is_brace(std::string s) {
	return s == "(" || s == ")";
}

bool is_comment(std::string s) {
	static const std::regex r(R"(//.*)");
	return std::regex_match(s, r);
}

token_type get_token_type(Trie* service_trie, std::string s) {
	if (is_comment(s)) return token_type::comment;
	if (is_service(service_trie, s)) return token_type::service;
	if (is_literal(s) != literal_type::none) return token_type::literal;
	if (is_identifier(s)) return token_type::identifier;
	if (is_operation(s)) return token_type::operation;
	if (is_punctuation(s)) return token_type::punctuation;
	if (is_comma(s)) return token_type::comma;
	if (is_brace(s)) return token_type::brace;
	return token_type::unknown;
}

void print_token(Trie* service_trie, std::string buffer) {
	if (buffer.empty()) return;
	auto type = get_token_type(service_trie, buffer);
	switch (type) {
	case service:
		std::cout << "(service, \"" << buffer << "\")\n";
		break;
	case identifier:
		std::cout << "(identifier, \"" << buffer << "\")\n";
		break;
	case literal:
		std::cout << "(literal, \"" << buffer << "\")\n";
		break;
	case operation:
		std::cout << "(operation, \"" << buffer << "\")\n";
		break;
	case punctuation:
		std::cout << "(punctuation, \"" << buffer << "\")\n";
		break;
	case comma:
		std::cout << "(comma, \"" << buffer << "\")\n";
		break;
	case brace:
		std::cout << "(brace, \"" << buffer << "\")\n";
		break;
	case comment:
		break;
	case unknown:
		std::cout << "(unknown, \"" << buffer << "\")\n";
		exit(0);
		break;
	}
}

int main() {
	Trie* service_trie = new Trie();
	std::ifstream file("ServiceWords.txt");
	std::string str;
	while (std::getline(file, str)) {
		if (str.empty()) continue;
		service_trie->add(str.begin(), str.end());
	}
	file.close();

	std::string code_path;
	std::cin >> code_path;

	file = std::ifstream(code_path);
	str.clear();

	std::string buffer = "";
	char c;
	token_type prev = token_type::unknown;
	bool prev_quotation = false, prev_single = false;
	while (file >> std::noskipws >> c) {
		if (c == ' ' || c == '\n') {
			if (prev == token_type::comment && c == ' ') continue;
			print_token(service_trie, buffer);
			prev = token_type::unknown;
			buffer.clear();
			continue;
		}
		buffer.push_back(c);
		auto type = get_token_type(service_trie, buffer);
		auto char_type = get_token_type(service_trie, std::string(1, c));
		if (type == token_type::unknown) {
			if (char_type == token_type::operation || char_type == token_type::punctuation || char_type == token_type::comma ||
				char_type == token_type::comment || char_type == token_type::brace) {
				buffer.pop_back();
				print_token(service_trie, buffer);
				prev = get_token_type(service_trie, buffer);
				buffer = std::string(1, c);
			} else if (prev == token_type::operation || prev == token_type::punctuation || prev == token_type::comma || prev == token_type::brace) {
				buffer.pop_back();
				print_token(service_trie, buffer);
				prev = get_token_type(service_trie, buffer);
				buffer = std::string(1, c);
			} else {
				if (c == '"') {
					if (!prev_quotation) {
						prev_quotation = true;
					}
				} else if (c == '\'') {
					if (!prev_single) {
						prev_single = true;
					}
				}
				if (prev_quotation || prev_single) continue;
				print_token(service_trie, buffer);
				exit(0);
			}
		}
		prev = get_token_type(service_trie, buffer);
	}

	print_token(service_trie, buffer);
}