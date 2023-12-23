#include <regex>
#include <iostream>
#include "LexicalAnalyzer.h"
#include <exception>
#include <stack>
#include "TableIdentifiers.h"

Token current_token;
std::stack<std::pair<ExprType, bool>> exprs;
TableIdentifiers* tid = new TableIdentifiers();
std::unordered_map<Function, TableIdentifiers*, FunctionHasher> funcs;

bool is_convertible(ExprType first, ExprType second) {
	switch (first) {
	case Bool:
		return second == ExprType::Bool;
	case Char:
	case Double:
	case UDouble:
	case Float:
	case UFloat:
	case Int:
	case UInt:
	case Long:
	case ULong:
	case Short:
	case UShort:
	case Byte:
		return second != ExprType::Bool && second != ExprType::String && second != ExprType::Void && second != ExprType::Unknown;
	case String:
		return second == ExprType::String;
	case Void:
	case Unknown:
		return second == ExprType::Unknown;
	default:
		break;
	}
}

ExprType calc_expr_type(ExprType first, ExprType second) {
	if (!is_convertible(first, second)) return ExprType::Unknown;
	if (type_prior[first] >= type_prior[second])
		return first;
	else
		return second;
}

ExprType string_to_type(std::string str) {
	if (str == "double") return ExprType::Double;
	if (str == "udouble") return ExprType::UDouble;
	if (str == "long") return ExprType::Long;
	if (str == "ulong") return ExprType::ULong;
	if (str == "float") return ExprType::Float;
	if (str == "ufloat") return ExprType::UFloat;
	if (str == "int") return ExprType::Int;
	if (str == "uint") return ExprType::UInt;
	if (str == "short") return ExprType::Short;
	if (str == "ushort") return ExprType::UShort;
	if (str == "byte") return ExprType::Byte;
	if (str == "char") return ExprType::Char;
	if (str == "string") return ExprType::String;
	if (str == "bool") return ExprType::Bool;
	if (str == "void") return ExprType::Void;
	return ExprType::Unknown;
}

std::string type_to_string(ExprType type) {
	switch (type) {
	case Bool:
		return "bool";
	case Char:
		return "char";
	case Double:
		return "double";
	case UDouble:
		return "udouble";
	case Float:
		return "float";
	case UFloat:
		return "ufloat";
	case Int:
		return "int";
	case UInt:
		return "uint";
	case Long:
		return "long";
	case ULong:
		return "ulong";
	case Short:
		return "short";
	case UShort:
		return "ushort";
	case Byte:
		return "byte";
	case String:
		return "string";
	case Void:
		return "void";
	case Unknown:
	default:
		return "unknown";
	}
}

void add_identifier(Identifier* ident) {
	if (tid->identifiers.count(ident->name) != 0) throw std::exception(("Identifier '" + ident->name + "' was already declared in the current scope").c_str());
	tid->identifiers[ident->name] = ident;
}

Identifier* get_identifier(std::string name) {
	TableIdentifiers* ptr = tid;
	while (ptr != nullptr) {
		if (ptr->identifiers.count(name) != 0) {
			return ptr->identifiers[name];
		}
		ptr = ptr->parent;
	}
	throw std::exception(("Identifier '" + name + "' was not declared in the current scope").c_str());
	return nullptr;
}

void del_table() {
	auto next = tid->parent;
	delete tid;
	tid = next;
	if (tid == nullptr)
		tid = new TableIdentifiers();
}

void create_table(bool new_func = false) {
	auto next = new TableIdentifiers();
	if (!new_func)
		next->parent = tid;
	tid = next;
}

void create_function(std::string name, ExprType type) {
	std::vector<Identifier*> idents;
	for (auto& u : tid->identifiers) {
		idents.push_back(u.second);
	}
	Function f = Function(name, type, idents);
	if (funcs.count(f) != 0)
		throw std::exception(("Function '" + f.name + "' was already declared in the current scope").c_str());
	funcs[f] = tid;
}

Function get_function(std::string name, std::vector<Identifier*> params) {
	Function f = Function(name, ExprType::Void, params);
	if (funcs.count(f) == 0)
		throw std::exception(("Function '" + f.name + "' was not declared in the current scope").c_str());
	auto res = funcs.find(f)->first;
	for (int i = 0; i < params.size(); ++i)
		delete params[i];
	return res;
}

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
void statement(LexicalAnalyzer& lex, bool prev_table = false);
void semicolon(LexicalAnalyzer& lex, bool is_important = false);
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

std::vector<std::string> service_types = { "bool", "char", "byte", "double", "udouble", "float", "ufloat", "int", "uint", "long",
		"ulong", "short", "ushort", "string" };

void semicolon(LexicalAnalyzer& lex, bool is_important) {
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
	if (!is_important) {
		may_be_semicolon(lex);
	}
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
	function_or_var_definitions(lex);
	may_be_semicolon(lex);
	program_body(lex);
}

void var_definition(LexicalAnalyzer& lex) {
	bool is_const = current_token.value == "const";
	ExprType curr_type;
	if (is_const) {
		current_token = lex.get_token();
		curr_type = string_to_type(current_token.value);
		current_token = lex.prev_token();
	} else
		curr_type = string_to_type(current_token.value);
	type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	std::string name = current_token.value;
	current_token = lex.get_token();
	if (current_token.value == "[") {
		current_token = lex.get_token();
		if (lex.is_literal(current_token.value) != lex.integer)
			throw std::exception("Invalid token: integer literal expected");
		int size = std::stoi(current_token.value);
		current_token = lex.get_token();
		if (current_token.value != "]")
			throw std::exception("Invalid token: ']' expected");
		current_token = lex.get_token();

		add_identifier(new Identifier(name, curr_type, is_const, true, size));
	} else {
		add_identifier(new Identifier(name, curr_type, is_const, false));
	}
	if (current_token.value == "=") {
		current_token = lex.get_token();
		expression(lex, true);
		if (!is_convertible(exprs.top().first, curr_type))
			throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(curr_type)).c_str());
		exprs.pop();
	}
}

void var_definitions(LexicalAnalyzer& lex, bool need_semicolon) {
	bool is_const = current_token.value == "const";
	ExprType curr_type;
	if (is_const) {
		current_token = lex.get_token();
		curr_type = string_to_type(current_token.value);
		current_token = lex.prev_token();
	} else
		curr_type = string_to_type(current_token.value);
	type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	std::string name = current_token.value;
	current_token = lex.get_token();
	if (current_token.value == "[") {
		current_token = lex.get_token();
		if (lex.is_literal(current_token.value) != lex.integer)
			throw std::exception("Invalid token: integer literal expected");
		int size = std::stoi(current_token.value);
		current_token = lex.get_token();
		if (current_token.value != "]")
			throw std::exception("Invalid token: ']' expected");
		current_token = lex.get_token();

		add_identifier(new Identifier(name, curr_type, is_const, true, size));
	} else {
		add_identifier(new Identifier(name, curr_type, is_const, false));
	}
	if (current_token.value == "=") {
		current_token = lex.get_token();
		expression(lex, true);

		if (!is_convertible(exprs.top().first, curr_type))
			throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(curr_type)).c_str());
		exprs.pop();
	}
	while (current_token.value == ",") {
		current_token = lex.get_token();
		if (current_token.type != LexicalAnalyzer::token_type::identifier)
			throw std::exception("Invalid token: identifier expected");
		name = current_token.value;
		current_token = lex.get_token();
		if (current_token.value == "[") {
			current_token = lex.get_token();
			if (lex.is_literal(current_token.value) != lex.integer)
				throw std::exception("Invalid token: integer literal expected");
			int size = std::stoi(current_token.value);
			current_token = lex.get_token();
			if (current_token.value != "]")
				throw std::exception("Invalid token: ']' expected");
			current_token = lex.get_token();

			add_identifier(new Identifier(name, curr_type, is_const, true, size));
		} else {
			add_identifier(new Identifier(name, curr_type, is_const, false));
		}
		if (current_token.value == "=") {
			current_token = lex.get_token();
			expression(lex, true);

			if (!is_convertible(exprs.top().first, curr_type))
				throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(curr_type)).c_str());
			exprs.pop();
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
	for (int i = 0; i < service_types.size(); ++i) {
		if (service_types[i] == current_token.value) {
			current_token = lex.get_token();
			return;
		}
	}
	throw std::exception("Invalid token: service type expected");
}

void function(LexicalAnalyzer& lex, bool is_struct) {
	ExprType curr_type = string_to_type(current_token.value);
	if (current_token.value == "void")
		current_token = lex.get_token();
	else
		default_type(lex);
	if (current_token.type != LexicalAnalyzer::token_type::identifier)
		throw std::exception("Invalid token: identifier expected");
	std::string name = current_token.value;
	current_token = lex.get_token();
	if (current_token.value != "(")
		throw std::exception("Invalid token: '(' expected");
	current_token = lex.get_token();

	create_table(true);
	bool was_parameters = false;
	check_function_brace:
	if (current_token.value == ")" || was_parameters) {
		create_function(name, curr_type);
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
		exprs.pop();
		current_token = lex.get_token();
		assignment_expression(lex);
	}
}

bool check_operation(ExprType first, ExprType second, std::string operation) {
	if (operation == "+")
		return first != ExprType::Bool && is_convertible(first, second);
	if (operation == "-" || operation == "*" || operation == "/" || operation == ">>" || operation == "<<" || operation == "<" || operation == "<=" ||
		operation == ">" || operation == ">=")
		return first != ExprType::Bool && first != ExprType::String && is_convertible(first, second);
	if (operation == "%")
		return first != ExprType::Bool && first != ExprType::String && first != ExprType::Float && first != ExprType::UFloat && 
		first != ExprType::Double && first != ExprType::UDouble && second != ExprType::Float && second != ExprType::UFloat &&
		second != ExprType::Double && second != ExprType::UDouble && is_convertible(first, second);
	if (operation == "&" || operation == "^" || operation == "|" || operation == "->" || operation == "~")
		return first != ExprType::String && is_convertible(first, second);
	if (operation == "=" || operation == "==" || operation == "!=")
		return is_convertible(first, second);
	if (operation == "||" || operation == "&&")
		return first == ExprType::Bool && is_convertible(first, second);
	return false;
}

void assignment_expression(LexicalAnalyzer& lex) {
	logical_or_expression(lex);
	while (current_token.value == "=" || current_token.value == "+=" || current_token.value == "-=" || current_token.value == "*=" ||
		current_token.value == "/=" || current_token.value == "%=" || current_token.value == ">>=" || current_token.value == "<<=" ||
		current_token.value == "&=" || current_token.value == "^=" || current_token.value == "|=" || current_token.value == "->=" ||
		current_token.value == "~=") {

		auto operation = current_token.value;
		auto tmp_operation = operation;
		if (operation != "=")
			operation = operation.substr(0, operation.size() - 1);
		if (!exprs.top().second)
			throw std::exception("Cannot apply assignment operator to rvalue");
		auto prev = exprs.top().first;
		exprs.pop();
		current_token = lex.get_token();
		logical_or_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + tmp_operation + " operator with " + type_to_string(prev) +
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), curr.second });
	}
}

void logical_or_expression(LexicalAnalyzer& lex) {
	logical_and_expression(lex);
	while (current_token.value == "||") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		logical_and_expression(lex);
		if (!check_operation(prev, exprs.top().first, "||"))
			throw std::exception(("Cannot apply || operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		exprs.pop();
		exprs.push({ ExprType::Bool, false });
	}
}

void logical_and_expression(LexicalAnalyzer& lex) {
	bitwise_or_expression(lex);
	while (current_token.value == "&&") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		bitwise_or_expression(lex);
		if (!check_operation(prev, exprs.top().first, "&&"))
			throw std::exception(("Cannot apply && operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		exprs.pop();
		exprs.push({ ExprType::Bool, false });
	}
}

void bitwise_or_expression(LexicalAnalyzer& lex) {
	bitwise_xor_expression(lex);
	while (current_token.value == "|") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		bitwise_xor_expression(lex);
		if (!check_operation(prev, exprs.top().first, "|"))
			throw std::exception(("Cannot apply | operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void bitwise_xor_expression(LexicalAnalyzer& lex) {
	bitwise_consequence_expression(lex);
	while (current_token.value == "^") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		bitwise_consequence_expression(lex);
		if (!check_operation(prev, exprs.top().first, "^"))
			throw std::exception(("Cannot apply ^ operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void bitwise_consequence_expression(LexicalAnalyzer& lex) {
	bitwise_and_expression(lex);
	while (current_token.value == "->") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		bitwise_and_expression(lex);
		if (!check_operation(prev, exprs.top().first, "->"))
			throw std::exception(("Cannot apply -> operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void bitwise_and_expression(LexicalAnalyzer& lex) {
	equality_expression(lex);
	while (current_token.value == "&") {
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		equality_expression(lex);
		if (!check_operation(prev, exprs.top().first, "&"))
			throw std::exception(("Cannot apply & operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void equality_expression(LexicalAnalyzer& lex) {
	comparison_expression(lex);
	while (current_token.value == "==" || current_token.value == "!=") {
		auto operation = current_token.value;
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		comparison_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) + 
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void comparison_expression(LexicalAnalyzer& lex) {
	bitwise_shift_expression(lex);
	while (current_token.value == ">" || current_token.value == "<" || current_token.value == ">=" || current_token.value == "<=") {
		auto operation = current_token.value;
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		bitwise_shift_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) +
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void bitwise_shift_expression(LexicalAnalyzer& lex) {
	plus_expression(lex);
	while (current_token.value == ">>" || current_token.value == "<<") {
		auto operation = current_token.value;
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		plus_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) +
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void plus_expression(LexicalAnalyzer& lex) {
	multiply_expression(lex);
	while (current_token.value == "+" || current_token.value == "-") {
		auto operation = current_token.value;
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		multiply_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) +
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void multiply_expression(LexicalAnalyzer& lex) {
	unary_expression(lex);
	while (current_token.value == "*" || current_token.value == "/" || current_token.value == "%") {
		auto operation = current_token.value;
		current_token = lex.get_token();
		auto prev = exprs.top().first;
		exprs.pop();
		unary_expression(lex);
		if (!check_operation(prev, exprs.top().first, operation))
			throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) +
				" and " + type_to_string(exprs.top().first)).c_str());
		auto curr = exprs.top();
		exprs.pop();
		exprs.push({ calc_expr_type(prev, curr.first), false });
	}
}

void unary_expression(LexicalAnalyzer& lex) {
	std::vector<std::string> operations;
	while (current_token.value == "+" || current_token.value == "-" || current_token.value == "!" || current_token.value == "~" ||
		current_token.value == "++" || current_token.value == "--") {
		operations.push_back(current_token.value);
		current_token = lex.get_token();
	}
	construct_expression(lex);
	auto curr = exprs.top();
	exprs.pop();
	for (int i = operations.size() - 1; i >= 0; --i) {
		if (operations[i] == "+" || operations[i] == "-" || operations[i] == "~") {
			if (curr.first == ExprType::String || curr.first == ExprType::Bool || curr.first == ExprType::Unknown || curr.first == ExprType::Void)
				throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
			curr.second = false;
		} else if (operations[i] == "!") {
			if (curr.first != ExprType::Bool)
				throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
			curr.second = false;
		} else {
			if (curr.first == ExprType::String || curr.first == ExprType::Bool || curr.first == ExprType::Unknown || curr.first == ExprType::Void)
				throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
			if (!curr.second)
				throw std::exception(("Cannot apply " + operations[i] + " operator to rvalue").c_str());
		}
	}
	exprs.push(curr);
}

void construct_expression(LexicalAnalyzer& lex) {
	if (current_token.type == LexicalAnalyzer::token_type::literal) {
		switch (lex.is_literal(current_token.value)) {
		case LexicalAnalyzer::literal_type::logical:
			exprs.push({ ExprType::Bool, false });
			break;
		case LexicalAnalyzer::literal_type::integer:
			exprs.push({ ExprType::Int, false });
			break;
		case LexicalAnalyzer::literal_type::real:
			exprs.push({ ExprType::Float, false });
			break;
		case LexicalAnalyzer::literal_type::symbol:
			exprs.push({ ExprType::Char, false });
			break;
		case LexicalAnalyzer::literal_type::string:
			exprs.push({ ExprType::String, false });
			break;
		default:
			exprs.push({ ExprType::Unknown, false });
			break;
		}
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
	std::string name = current_token.value;
	current_token = lex.get_token();
	Identifier* ident = nullptr;
	if (current_token.value == "[")
		ident = get_identifier(current_token.value);
	while (current_token.value == "[" || current_token.value == "(") { // TODO: field .
		if (current_token.value == "[") {
			if (!ident->is_array)
				throw std::exception("Cannot apply operator [] to anything other than an array");
			current_token = lex.get_token();
			expression(lex, true);
			if (!is_convertible(exprs.top().first, ExprType::Long))
				throw std::exception("Cannot access index that is not an integer");
			exprs.pop();
			if (current_token.value != "]")
				throw std::exception("Invalid token: ']' expected");
			exprs.push({ ident->type, true });
			current_token = lex.get_token();
			return;
		} /*else if (current_token.value == ".") {
			current_token = lex.get_token();
			if (current_token.type != LexicalAnalyzer::token_type::identifier)
				throw std::exception("Invalid token: identifier expected");
			current_token = lex.get_token();
		} */ else { // TODO: check function
			current_token = lex.get_token();
			std::vector<Identifier*> idents;
			if (current_token.value != ")") {
				expression(lex, true);
				idents.push_back(new Identifier(std::string(""), exprs.top().first, false, false));
				exprs.pop();
			}
			while (current_token.value == ",") {
				current_token = lex.get_token();
				expression(lex, true);
				idents.push_back(new Identifier(std::string(""), exprs.top().first, false, false));
				exprs.pop();
			}
			if (current_token.value != ")")
				throw std::exception("Invalid token: ')' expected");
			current_token = lex.get_token();

			auto func = get_function(name, idents);
			exprs.push({ func.type, false });
			return;
		}
	}

	ident = get_identifier(name);
	exprs.push({ ident->type, true });
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
	} else if (current_token.value == "elif") {
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
	create_table();
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
	semicolon(lex, true);
	if (current_token.value != ";") {
		expression(lex);
	}
	semicolon(lex, true);
	if (current_token.value != ")") {
		expression(lex);
	}
	if (current_token.value != ")")
		throw std::exception("Invalid token: ')' expected");
	current_token = lex.get_token();
	statement(lex, true);
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

void statement(LexicalAnalyzer& lex, bool prev_table) {
	if (current_token.value == "{") {
		current_token = lex.get_token();
		if (!prev_table)
			create_table();
		while (current_token.value != "}" && current_token.line != -1)
			statement(lex);
		if (current_token.value != "}")
			throw std::exception("Invalid token: '}' expected");
		current_token = lex.get_token();
		del_table();
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