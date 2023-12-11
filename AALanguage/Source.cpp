#include <regex>
#include <iostream>
#include "LexicalAnalyzer.h"
#include <exception>

Token current_token;

void program(LexicalAnalyzer& lex);
void preprocessor(LexicalAnalyzer& lex);
void namespace_definition(LexicalAnalyzer& lex);
void program_body(LexicalAnalyzer& lex);
void var_definition(LexicalAnalyzer& lex);
void var_definitions(LexicalAnalyzer& lex, bool need_semicolon = true);
void type(LexicalAnalyzer& lex);
void default_type(LexicalAnalyzer& lex);
void function_or_var_definitions(LexicalAnalyzer& lex, bool is_struct = false);
void function(LexicalAnalyzer& lex, bool is_struct = false);
void expression(LexicalAnalyzer& lex, bool is_vars = false);
void parameter_list(LexicalAnalyzer& lex);
void statement(LexicalAnalyzer& lex);
void semicolon(LexicalAnalyzer& lex);
void may_be_semicolon(LexicalAnalyzer& lex);

void assignment_expression(LexicalAnalyzer& lex);
void logical_or_expression(LexicalAnalyzer& lex);
void logical_and_expression(LexicalAnalyzer& lex);
void bitwise_or_expression(LexicalAnalyzer& lex);
void bitwise_xor_expression(LexicalAnalyzer& lex);
void bitwise_consequence_expression(LexicalAnalyzer& lex);
void bitwise_and_expression(LexicalAnalyzer& lex);
void equality_expression(LexicalAnalyzer& lex);
void comparison_expression(LexicalAnalyzer& lex);
void bitwise_shift_expression(LexicalAnalyzer& lex);
void plus_expression(LexicalAnalyzer& lex);
void multiply_expression(LexicalAnalyzer& lex);
void unary_expression(LexicalAnalyzer& lex);
void construct_expression(LexicalAnalyzer& lex);
void field(LexicalAnalyzer& lex);

void if_statement(LexicalAnalyzer& lex);
void goto_statement(LexicalAnalyzer& lex);
void return_statement(LexicalAnalyzer& lex);
void while_statement(LexicalAnalyzer& lex);
void for_statement(LexicalAnalyzer& lex);
void switch_statement(LexicalAnalyzer& lex);
void struct_statement(LexicalAnalyzer& lex);

std::vector<std::string> service_types = { "bool", "char", "uchar", "double", "udouble", "float", "ufloat", "int", "uint", "long",
		"ulong", "short", "ushort", "string" };

void semicolon(LexicalAnalyzer& lex) {
	if (current_token.line == -1)
		throw std::exception("Invalid end of line: ';' expected");
	if (current_token.value != ";")
		throw std::exception("Invalid token: ';' expected");
	int curr = current_token.line;
	int prev = lex.prev_token().line;
	lex.get_token();
	if (curr != prev) {
		current_token = lex.prev_token();
		throw std::exception("Invalid end of line: ';' expected");
	}
	current_token = lex.get_token();
	may_be_semicolon(lex);
}

void may_be_semicolon(LexicalAnalyzer& lex) {
	while (current_token.value == ";") {
		current_token = lex.get_token();
	}
}

void program(LexicalAnalyzer& lex) {
	may_be_semicolon(lex);
	preprocessor(lex);
	if (current_token.line == -1) return;
	program_body(lex);
}

void preprocessor(LexicalAnalyzer& lex) {
	while (current_token.value == "using") {
		current_token = lex.get_token();
		namespace_definition(lex);
		semicolon(lex);
		may_be_semicolon(lex);
	}
	may_be_semicolon(lex);
}

void namespace_definition(LexicalAnalyzer& lex) {
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	while (current_token.value == ".") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::identifier) {
			throw std::exception("Invalid token: identifier expected");
		}
		current_token = lex.get_token();
	}
}

void program_body(LexicalAnalyzer& lex) {
	if (current_token.line == -1) return;
	if (current_token.value == "struct") {
		struct_statement(lex);
		semicolon(lex);
		may_be_semicolon(lex);
		program_body(lex);
		return;
	}
	function_or_var_definitions(lex);
	may_be_semicolon(lex);
	program_body(lex);
}

void var_definition(LexicalAnalyzer& lex) {
	type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	if (current_token.value == "[") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::literal)
			throw std::exception("Invalid token: literal expected");
		current_token = lex.get_token();
		if (current_token.value != "]")
			throw std::exception("Invalid token: ']' expected");
		current_token = lex.get_token();
	}
	if (current_token.value == "=") {
		current_token = lex.get_token();
		expression(lex, true);
	}
}

void var_definitions(LexicalAnalyzer& lex, bool need_semicolon) {
	type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	if (current_token.value == "[") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::literal)
			throw std::exception("Invalid token: literal expected");
		current_token = lex.get_token();
		if (current_token.value != "]")
			throw std::exception("Invalid token: ']' expected");
		current_token = lex.get_token();
	}
	if (current_token.value == "=") {
		current_token = lex.get_token();
		expression(lex, true);
	}
	while (current_token.value == ",") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::identifier)
			throw std::exception("Invalid token: identifier expected");
		current_token = lex.get_token();
		if (current_token.value == "[") {
			current_token = lex.get_token();
			if (current_token.type != LexicalAnalyzer::token_type::literal)
				throw std::exception("Invalid token: literal expected");
			current_token = lex.get_token();
			if (current_token.value != "]")
				throw std::exception("Invalid token: ']' expected");
			current_token = lex.get_token();
		}
		if (current_token.value == "=") {
			current_token = lex.get_token();
			expression(lex);
		}
	}
	if (need_semicolon) {
		semicolon(lex);
		may_be_semicolon(lex);
	}
}

void type(LexicalAnalyzer& lex) {
	if (current_token.value == "const")
		current_token = lex.get_token();
	default_type(lex);
}

void default_type(LexicalAnalyzer& lex) {
	if (current_token.type != LexicalAnalyzer::token_type::identifier) {
		for (int i = 0; i < service_types.size(); ++i) {
			if (service_types[i] == current_token.value) {
				current_token = lex.get_token();
				return;
			}
		}
		throw std::exception("Invalid token: identifier expected");
	}
	current_token = lex.get_token();
}

void function(LexicalAnalyzer& lex, bool is_struct) {
	if (current_token.value == "void")
		current_token = lex.get_token();
	else if (is_struct && current_token.value == "constructor")
		current_token = lex.get_token();
	else if (is_struct && current_token.value == "destructor")
		current_token = lex.get_token();
	else
		default_type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();

	bool was_parameters = false;
	check_function_brace:
	if (current_token.value == ")" || was_parameters) {
		current_token = lex.get_token();
		if (current_token.value != "{")
			throw std::exception("Invalid token: '{' expected");
		current_token = lex.get_token();
		while (current_token.value != "}" && current_token.line != -1)
			statement(lex);
		if (current_token.value != "}")
			throw std::exception("Invalid token: '}' expected");
		current_token = lex.get_token();
		may_be_semicolon(lex);
	} else {
		parameter_list(lex);
		was_parameters = true;
		goto check_function_brace;
	}
}

void function_or_var_definitions(LexicalAnalyzer& lex, bool is_struct) {
	if (current_token.value == "const") {
		var_definitions(lex);
		return;
	}
	if (current_token.value == "constructor") {
		function(lex, is_struct);
		return;
	}
	if (current_token.value == "destructor") {
		function(lex, is_struct);
		return;
	}
	if (current_token.value == "void") {
		function(lex, is_struct);
		return;
	}
	default_type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	if (current_token.value == "(") {
		current_token = lex.prev_token();
		current_token = lex.prev_token();
		function(lex, is_struct);
	} else if (current_token.value == "=" || current_token.value == "," || current_token.value == ";") {
		current_token = lex.prev_token();
		current_token = lex.prev_token();
		var_definitions(lex);
	} else
		throw std::exception("Invalid token: '=', ',', ';' or '(' expected");
}

void expression(LexicalAnalyzer& lex, bool is_vars) {
	assignment_expression(lex);
	while (!is_vars && current_token.value == ",") {
		current_token = lex.get_token();
		assignment_expression(lex);
	}
}

void assignment_expression(LexicalAnalyzer& lex) {
	logical_or_expression(lex);
	while (current_token.value == "=" || current_token.value == "+=" || current_token.value == "-=" || current_token.value == "*=" ||
		current_token.value == "/=" || current_token.value == "%=" || current_token.value == ">>=" || current_token.value == "<<=" ||
		current_token.value == "&=" || current_token.value == "^=" || current_token.value == "|=" || current_token.value == "->=" ||
		current_token.value == "~=") {
		current_token = lex.get_token();
		logical_or_expression(lex);
	}
}

void logical_or_expression(LexicalAnalyzer& lex) {
	logical_and_expression(lex);
	while (current_token.value == "||") {
		current_token = lex.get_token();
		logical_and_expression(lex);
	}
}

void logical_and_expression(LexicalAnalyzer& lex) {
	bitwise_or_expression(lex);
	while (current_token.value == "&&") {
		current_token = lex.get_token();
		bitwise_or_expression(lex);
	}
}

void bitwise_or_expression(LexicalAnalyzer& lex) {
	bitwise_xor_expression(lex);
	while (current_token.value == "|") {
		current_token = lex.get_token();
		bitwise_xor_expression(lex);
	}
}

void bitwise_xor_expression(LexicalAnalyzer& lex) {
	bitwise_consequence_expression(lex);
	while (current_token.value == "^") {
		current_token = lex.get_token();
		bitwise_consequence_expression(lex);
	}
}

void bitwise_consequence_expression(LexicalAnalyzer& lex) {
	bitwise_and_expression(lex);
	while (current_token.value == "->") {
		current_token = lex.get_token();
		bitwise_and_expression(lex);
	}
}

void bitwise_and_expression(LexicalAnalyzer& lex) {
	equality_expression(lex);
	while (current_token.value == "&") {
		current_token = lex.get_token();
		equality_expression(lex);
	}
}

void equality_expression(LexicalAnalyzer& lex) {
	comparison_expression(lex);
	while (current_token.value == "==" || current_token.value == "!=") {
		current_token = lex.get_token();
		comparison_expression(lex);
	}
}

void comparison_expression(LexicalAnalyzer& lex) {
	bitwise_shift_expression(lex);
	while (current_token.value == ">" || current_token.value == "<" || current_token.value == ">=" || current_token.value == "<=") {
		current_token = lex.get_token();
		bitwise_shift_expression(lex);
	}
}

void bitwise_shift_expression(LexicalAnalyzer& lex) {
	plus_expression(lex);
	while (current_token.value == ">>" || current_token.value == "<<") {void field(LexicalAnalyzer& lex);
		current_token = lex.get_token();
		plus_expression(lex);
	}
}

void plus_expression(LexicalAnalyzer& lex) {
	multiply_expression(lex);
	while (current_token.value == "+" || current_token.value == "-") {
		current_token = lex.get_token();
		multiply_expression(lex);
	}
}

void multiply_expression(LexicalAnalyzer& lex) {
	unary_expression(lex);
	while (current_token.value == "*" || current_token.value == "/" || current_token.value == "%") {
		current_token = lex.get_token();
		unary_expression(lex);
	}
}

void unary_expression(LexicalAnalyzer& lex) {
	while (current_token.value == "+" || current_token.value == "-" || current_token.value == "!" || current_token.value == "~" ||
		current_token.value == "++" || current_token.value == "--") {
		current_token = lex.get_token();
	}
	construct_expression(lex);
}

void construct_expression(LexicalAnalyzer& lex) {
	if (current_token.type == LexicalAnalyzer::token_type::literal) {
		current_token = lex.get_token();
		return;
	}
	if (current_token.value == "(") {
		current_token = lex.get_token();
		expression(lex);
		if (current_token.value != ")")
			throw std::exception("Invalid token: ')' expected");
		current_token = lex.get_token();
		return;
	}
	field(lex);
}

void field(LexicalAnalyzer& lex) {
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	while (current_token.value == "[" || current_token.value == "." || current_token.value == "(") {
		if (current_token.value == "[") {
			current_token = lex.get_token();
			expression(lex, true);
			if (current_token.value != "]")
				throw std::exception("Invalid token: ']' expected");
			current_token = lex.get_token();

		} else if (current_token.value == ".") {
			current_token = lex.get_token();
			if (current_token.type != LexicalAnalyzer::token_type::identifier)
				throw std::exception("Invalid token: identifier expected");
			current_token = lex.get_token();
		} else {
			current_token = lex.get_token();
			if (current_token.value != ")") {
				expression(lex, true);
			}
			while (current_token.value == ",") {
				current_token = lex.get_token();
				expression(lex, true);
			}
			if (current_token.value != ")")
				throw std::exception("Invalid token: ')' expected");
			current_token = lex.get_token();
		}
	}
}

void parameter_list(LexicalAnalyzer& lex) {
	var_definition(lex);
	while (current_token.value == ",") {
		current_token = lex.get_token();
		var_definition(lex);
	}
}

void if_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "if")
		throw std::exception("Invalid token: 'if' expected");
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();
	expression(lex);
	if (current_token.value != ")")
		throw std::exception("Invalid token: ')' expected");
	current_token = lex.get_token();
	statement(lex);
	if (current_token.value == "else") {
		current_token = lex.get_token();
		statement(lex);
	}
}

void goto_statement(LexicalAnalyzer& lex) {
	if (current_token.value == "break") {
		current_token = lex.get_token();
	} else if (current_token.value == "continue") {
		current_token = lex.get_token();
	} else if (current_token.value == "goto") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::identifier)
			throw std::exception("Invalid token: 'identifier' expected");
		current_token = lex.get_token();
	} else
		throw std::exception("Invalid token: 'break', 'continue' or 'goto' expected");
}

void return_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "return")
		throw std::exception("Invalid token: 'return' expected");
	current_token = lex.get_token();
	if (current_token.value != ";")
		expression(lex);
}

void while_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "while")
		throw std::exception("Invalid token: 'while' expected");
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();
	expression(lex);
	if (current_token.value != ")")
		throw std::exception("Invalid token: ')' expected");
	current_token = lex.get_token();
	statement(lex);
}

void for_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "for")
		throw std::exception("Invalid token: 'for' expected");
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();
	if (current_token.value != ";") {
		if (current_token.type == LexicalAnalyzer::token_type::service) {
			for (int i = 0; i < service_types.size(); ++i) {
				if (service_types[i] == current_token.value) {
					var_definitions(lex, false);
					break;
				}
			}
		}
		else if (current_token.value == "const")
			var_definitions(lex, false);
		else {
			if (current_token.type == LexicalAnalyzer::token_type::identifier) {
				current_token = lex.get_token();
				if (current_token.type == LexicalAnalyzer::token_type::identifier) {
					current_token = lex.prev_token();
					var_definitions(lex, false);
			
				} else {
					current_token = lex.prev_token();
					expression(lex);
				}
			}
		}
	}
	semicolon(lex);
	if (current_token.value != ";") {
		expression(lex);
	}
	semicolon(lex);
	if (current_token.value != ")") {
		expression(lex);
	}
	if (current_token.value != ")")
		throw std::exception("Invalid token: ')' expected");
	current_token = lex.get_token();
	statement(lex);
}

void switch_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "switch")
		throw std::exception("Invalid token: 'switch' expected");
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();
	expression(lex);
	if (current_token.value != ")")
		throw std::exception("Invalid token: ')' expected");
	current_token = lex.get_token();
	if (current_token.value != "{")
		throw std::exception("Invalid token: '{' expected");
	current_token = lex.get_token();
	while (current_token.value == "case" || current_token.value == "default") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::literal)
			throw std::exception("Invalid token: 'literal' expected");
		current_token = lex.get_token();
		if (current_token.value != ":")
			throw std::exception("Invalid token: ':' expected");
		current_token = lex.get_token();
		statement(lex);
	}
	if (current_token.value != "}")
		throw std::exception("Invalid token: '}' expected");
	current_token = lex.get_token();
}

void struct_statement(LexicalAnalyzer& lex) {
	if (current_token.value != "struct")
		throw std::exception("Invalid token: 'struct' expected");
	current_token = lex.get_token();
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	current_token = lex.get_token();
	if (current_token.value != "{")
		throw std::exception("Invalid token: '{' expected");
	current_token = lex.get_token();
	while (current_token.value != "}" && current_token.line != -1) {
		function_or_var_definitions(lex, true);
	}
	if (current_token.value != "}")
		throw std::exception("Invalid token: '}' expected");
	current_token = lex.get_token();
}

void statement(LexicalAnalyzer& lex) {
	if (current_token.value == "{") {
		current_token = lex.get_token();
		while (current_token.value != "}" && current_token.line != -1)
			statement(lex);
		if (current_token.value != "}")
			throw std::exception("Invalid token: '}' expected");
		current_token = lex.get_token();
		may_be_semicolon(lex);
		return;
	}
	if (current_token.value == "if") {
		if_statement(lex);
		may_be_semicolon(lex);
		return;
	}
	if (current_token.value == "goto" || current_token.value == "break" || current_token.value == "continue") {
		goto_statement(lex);
		semicolon(lex);
		may_be_semicolon(lex);
		return;
	}
	if (current_token.value == "return") {
		return_statement(lex);
		semicolon(lex);
		return;
		may_be_semicolon(lex);
	}
	if (current_token.value == "while") {
		while_statement(lex);
		may_be_semicolon(lex);
		return;
	}
	if (current_token.value == "for") {
		for_statement(lex);
		may_be_semicolon(lex);
		return;
	}
	if (current_token.value == "switch") {
		switch_statement(lex);
		may_be_semicolon(lex);
		return;
	}
	if (current_token.type == LexicalAnalyzer::token_type::service) {
		for (int i = 0; i < service_types.size(); ++i) {
			if (service_types[i] == current_token.value) {
				var_definitions(lex);
				return;
			}
		}
	}
	if (current_token.value == "const") {
		var_definitions(lex);
		return;
	}
	if (current_token.type == LexicalAnalyzer::token_type::identifier) {
		current_token = lex.get_token();
		if (current_token.type == LexicalAnalyzer::token_type::identifier) {
			current_token = lex.prev_token();
			var_definitions(lex);
		} else {
			current_token = lex.prev_token();
			expression(lex);
			semicolon(lex);
			may_be_semicolon(lex);
		}
	} else {
		if (current_token.value != ";") {
			expression(lex);
			semicolon(lex);
			may_be_semicolon(lex);
		} else {
			may_be_semicolon(lex);
		}
	}
}

int main() {
	std::string path;
	std::cin >> path;
	LexicalAnalyzer lex(path);

	std::cout << "The lexical analysis was successful\n";

	try {
		current_token = lex.get_token();
		program(lex);

		if (current_token.line != -1) {
			std::cout << "Unexpected end of file";
			exit(0);
		}
		std::cout << "The syntactic analysis was successful";
	} catch (std::exception ex) {
		std::cout << ex.what();
		if (current_token.line != -1) {
			std::cout << " (" << current_token.line << " line)";
		}
	}
}