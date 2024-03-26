#pragma once

#include <regex>
#include <iostream>
#include "LexicalAnalyzer.h"
#include <exception>
#include <stack>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include "Large.h"
#include "Poliz.h"
#include "Semantic.h"

class Analyzer {
public:
    Analyzer();
    void execute(std::string path, std::string lib_path);
private:
    Token current_token;
    Poliz prog;
    Semantic semantic;
    std::vector<std::string> service_types = { "bool", "char", "byte", "double", "udouble", "float", "ufloat", "int", "uint", "long",
        "ulong", "short", "ushort", "string", "large" };

    void program(LexicalAnalyzer& lex);
    void preprocessor(LexicalAnalyzer& lex);
    void namespace_definition(LexicalAnalyzer& lex);
    void program_body(LexicalAnalyzer& lex);
    void var_definition(LexicalAnalyzer& lex);
    void var_definitions(LexicalAnalyzer& lex, bool need_semicolon = true, bool is_program = false);
    void type(LexicalAnalyzer& lex);
    void default_type(LexicalAnalyzer& lex);
    void function_or_var_definitions(LexicalAnalyzer& lex, bool is_program = false);
    void function(LexicalAnalyzer& lex, bool is_struct = false);
    void expression(LexicalAnalyzer& lex, bool is_vars = false, bool is_print = false);
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
    void field(LexicalAnalyzer& lex, bool only_array = false);

    void if_statement(LexicalAnalyzer& lex);
    void goto_statement(LexicalAnalyzer& lex);
    void return_statement(LexicalAnalyzer& lex);
    void while_statement(LexicalAnalyzer& lex);
    void for_statement(LexicalAnalyzer& lex);
    void switch_statement(LexicalAnalyzer& lex);

    void read_statement(LexicalAnalyzer& lex);
    void readln_statement(LexicalAnalyzer& lex);
    void print_statement(LexicalAnalyzer& lex);
    void exit_statement(LexicalAnalyzer& lex);
    void convert_statement(LexicalAnalyzer& lex);
    void rand_statement(LexicalAnalyzer& lex);
    void strlen_statement(LexicalAnalyzer& lex);
    void time_statement(LexicalAnalyzer& lex);
};