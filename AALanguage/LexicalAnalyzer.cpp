#include "LexicalAnalyzer.h"
#include <iostream>
#include <fstream>

bool LexicalAnalyzer::is_service(Trie* service_trie, std::string s) {
	return service_trie->find(s.begin(), s.end()) != nullptr;
}

bool LexicalAnalyzer::is_identifier(std::string s) {
	static const std::regex r(R"([a-zA-Z_][\w]*)");
	return std::regex_match(s, r);
}

LexicalAnalyzer::literal_type LexicalAnalyzer::is_literal(std::string s) {
	if (s == "true" || s == "false") return literal_type::logical;

	static const std::regex integer_r(R"([0-9]{1,}(UL|ul|L|l|I|i|U|u|s|S)?)");
	if (std::regex_match(s, integer_r)) return literal_type::integer;

	static const std::regex real_r(R"(([0-9]{1,}\.[0-9]*)(e(\+|-)?[0-9]{1,})?(d|D|f|F)?)");
	if (std::regex_match(s, real_r)) return literal_type::real;

	static const std::regex symbol_r(R"(\'(.|\\n|\\t|\\0|\\'|\\")\')");
	if (std::regex_match(s, symbol_r)) return literal_type::symbol;

	static const std::regex string_r(R"(".*")");
	if (std::regex_match(s, string_r)) return literal_type::string;

	return literal_type::none;
}

bool LexicalAnalyzer::is_operation(std::string s) {
	return s == "[" || s == "]" || s == "." || s == "++" || s == "--" || s == "+" || s == "-" || s == "!" || s == "~" || s == "*" || s == "/" || s == "%" ||
		s == ">>" || s == "<<" || s == ">" || s == ">=" || s == "<" || s == "<=" || s == "==" || s == "!=" || s == "&" || s == "^" || s == "|" || s == "->" ||
		s == "&&" || s == "**" || s == "||" || s == "=" || s == "+=" || s == "-=" || s == "*=" || s == "/=" || s == "%=" || s == ">>=" || s == "<<=" ||
		s == "&=" || s == "^=" || s == "|=" || s == "->=" || s == "~=";
}

bool LexicalAnalyzer::is_punctuation(std::string s) {
	return s == ":" || s == ";" || s == "{" || s == "}";
}

bool LexicalAnalyzer::is_comma(std::string s) {
	return s == ",";
}

bool LexicalAnalyzer::is_brace(std::string s) {
	return s == "(" || s == ")";
}

bool LexicalAnalyzer::is_comment(std::string s) {
	static const std::regex r(R"(//.*)");
	return std::regex_match(s, r);
}

LexicalAnalyzer::token_type LexicalAnalyzer::get_token_type(Trie* service_trie, std::string s) {
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

void LexicalAnalyzer::print_token(Trie* service_trie, std::string buffer) {
	if (buffer.empty()) return;
	auto type = LexicalAnalyzer::get_token_type(service_trie, buffer);
	if (type == unknown) {
		std::cout << "(unknown, \"" << buffer << "\", line " << line_ << ")\n";
		ok_ = false;
		exit(0);
	}
	tokens_.push_back(Token(type, buffer, line_));
}

Token LexicalAnalyzer::get_token() {
	if (curr_ == tokens_.size()) {
		return Token(unknown, "", -1);
	}
	return tokens_[curr_++];
}

Token LexicalAnalyzer::prev_token() {
	if (curr_ <= 1) return tokens_[0];
	return tokens_[--curr_ - 1];
}

bool LexicalAnalyzer::get_ok() const {
	return ok_;
}

LexicalAnalyzer::LexicalAnalyzer(std::string path) {
	Trie* service_trie = new Trie();
	std::ifstream file("ServiceWords.txt");
	std::string str;
	while (std::getline(file, str)) {
		if (str.empty()) continue;
		service_trie->add(str.begin(), str.end());
	}
	file.close();

	file = std::ifstream(path);

	str.clear();

	std::string buffer = "";
	char c;
	token_type prev = token_type::unknown;
	bool prev_quotation = false, prev_single = false;
	while (file >> std::noskipws >> c) {
		if (c == '\t') c = ' ';

		if (c == ' ' && !prev_quotation && !prev_single || c == '\n' || buffer.empty() && (is_punctuation(std::string(1, c)) ||
			c == ',' || c == '(' || c == ')')) {
			if (buffer.empty() && (is_punctuation(std::string(1, c)) || c == ',' || c == '(' || c == ')'))
				buffer.push_back(c);
			if (c == '\n') prev_quotation = prev_single = false;
			if (prev == token_type::comment && c == ' ') continue;
			print_token(service_trie, buffer);
			prev = token_type::unknown;
			buffer.clear();
			if (c == '\n') ++line_;
			continue;
		}
		buffer.push_back(c);
		auto type = get_token_type(service_trie, buffer);
		auto char_type = get_token_type(service_trie, std::string(1, c));
		if (type == token_type::unknown) {
			if (prev_quotation || prev_single) continue;
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
					prev_quotation = !prev_quotation;
				} else if (c == '\'') {
					prev_single = !prev_single;
				}
				if (prev_quotation || prev_single) continue;
				print_token(service_trie, buffer);
				exit(0);
			}
		} else if (prev_quotation || prev_single) {
			prev_quotation = prev_single = false;
			print_token(service_trie, buffer);
			prev = token_type::unknown;
			buffer.clear();
			continue;
		}
		prev = get_token_type(service_trie, buffer);
	}

	print_token(service_trie, buffer);
}