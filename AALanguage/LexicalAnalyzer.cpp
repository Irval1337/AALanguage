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

	static const std::regex integer_r(R"([0-9]{1,}(UL|ul|L|l|I|i|U|u|s|S|us|US|b|B|ui|UI)?)");
	if (std::regex_match(s, integer_r)) return literal_type::integer;

	static const std::regex real_r(R"(([0-9]{1,}\.[0-9]*)(e(\+|-)?[0-9]{1,})?(d|D|f|F|ud|UD|uf|UF)?)");
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
		s == "&&" || s == "||" || s == "=" || s == "+=" || s == "-=" || s == "*=" || s == "/=" || s == "%=" || s == ">>=" || s == "<<=" ||
		s == "&=" || s == "^=" || s == "|=" || s == "->=";
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
		printf(("\x1B[31m(unknown, \"" + buffer + "\", line " + std::to_string(line_) + ")\n\033[0m").c_str());
		ok_ = false;
		exit(0);
	}
	if (type == comment) return;
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

std::vector<Token> LexicalAnalyzer::get_tokens() {
	return tokens_;
}

std::vector<Token> LexicalAnalyzer::merge(std::vector<Token>& f, std::vector<Token>& s, int& fin_ind, int start_ptr) {
	std::vector<Token> res;
	for (int i = 0; i < start_ptr; ++i) {
		res.push_back(f[i]);
	}
	for (int i = 0; i < s.size(); ++i) {
		res.push_back(s[i]);
	}
	for (int i = fin_ind + 1; i < f.size(); ++i) {
		res.push_back(f[i]);
	}
	fin_ind += s.size();
	return res;
}

LexicalAnalyzer::LexicalAnalyzer(std::string path, LibParser& lib_parser) {
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

		if (buffer == "\"")
			prev_quotation = true;
		if (buffer == "\'")
			prev_single = true;

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

	int start_ptr = 0;
	int ptr = 0;
	int state = 0;
	int state_line = -1;
	std::string lib_buff = "";
	while (ptr < tokens_.size()) {
		if (state == 0) {
			if (tokens_[ptr].value == "using") {
				state = 1;
				start_ptr = ptr;
				state_line = tokens_[ptr].line;
				lib_buff.clear();
			}
			++ptr;
		} else if (state == 1) {
			if (tokens_[ptr].line != state_line)
				throw std::exception(("Invalid token: ';' expected (" + std::to_string(tokens_[ptr].line) + " line)").c_str());
			if (tokens_[ptr].value != ";") {
				lib_buff += tokens_[ptr].value;
			} else {
				bool need_to_parse = lib_parser.add_lib(lib_buff);
				if (need_to_parse) {
					std::string lib_path = lib_parser.name_to_path(lib_buff);
					auto lib_tokens = LexicalAnalyzer(lib_path, lib_parser).get_tokens();
					tokens_ = merge(tokens_, lib_tokens, ptr, start_ptr);
				}
				state = 0;
			}
			++ptr;
		}
	}
	if (state != 0)
		throw std::exception(("Invalid token: ';' expected (" + std::to_string(tokens_[tokens_.size() - 1].line) + " line)").c_str());
}