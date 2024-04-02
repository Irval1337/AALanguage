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

//! The main class that implements syntactic analysis of the program by array of tokens. During its execution other components of the interpreter are called.
class Analyzer {
public:
    //! Default constructor, which initializes the POLIZ generator and the semantic analyzer.
    Analyzer();
    /**
    * @brief A function that starts full analysis and generation of a file with specified parameters.
    *
    * @param path Path to the AALanguage program 
    * @param lib_path Path to the directory with built-in libraries
    */
    void execute(std::string path, std::string lib_path);
private:
    Token current_token;
    Poliz prog;
    Semantic semantic;
    //! All built-in language types
    std::vector<std::string> service_types = { "bool", "char", "byte", "double", "udouble", "float", "ufloat", "int", "uint", "long",
        "ulong", "short", "ushort", "string", "large" };

    /**
    * <program> ::= <preprocessor> <program-body>
    */
    void program(LexicalAnalyzer& lex);
    /**
    * <preprocessor> ::= {"using" <namespace-definiton>";"}
    */
    void preprocessor(LexicalAnalyzer& lex);
    /**
    * <namespace-definiton> ::= <identifier> | <identifier>"."<namespace-definiton>
    */
    void namespace_definition(LexicalAnalyzer& lex);
    /**
    * <program-body> ::= [<function>|<var-definitons>|<struct-statement>]{<program-body>}
    */
    void program_body(LexicalAnalyzer& lex);
    /**
    * <var-definition> ::= <type> (<identifier> | <identifier>"="<expression>)
    */
    void var_definition(LexicalAnalyzer& lex);
    /**
    * <var-definitions> ::= <type> (<identifier> | <identifier>"="<expression>){"," <identifier> | <identifier>"="<expression>}";"
    */
    void var_definitions(LexicalAnalyzer& lex, bool need_semicolon = true, bool is_program = false);
    /**
    * <type> ::= <const-type> | <default-type>
    * 
    * <const-type> ::= "const" <default-type>
    */
    void type(LexicalAnalyzer& lex);
    /**
    * <default-type> ::= <identifier>
    */
    void default_type(LexicalAnalyzer& lex);
    void function_or_var_definitions(LexicalAnalyzer& lex, bool is_program = false);
    /**
    * <function> ::= <type> <identifier>"("[parameter-list]")" "{" {<statement>} "}"
    */
    void function(LexicalAnalyzer& lex, bool is_struct = false);
    /**
    * <expression> ::= <assignment-expression>{"," <assignment-expression>}
    */
    void expression(LexicalAnalyzer& lex, bool is_vars = false, bool is_print = false);
    /**
    * <parameter-list> ::= <var-definition> {","<var-definition>}
    */
    void parameter_list(LexicalAnalyzer& lex);
    /**
    * <statement> ::= "{"{<statement>}"}"  | <var-definitions> | <expression-statement> | <if-statement> | <goto-statement>| <return-statement> | <while-statement> | <for-statement> | <switch-statement>
    */
    void statement(LexicalAnalyzer& lex, bool prev_table = false);
    void semicolon(LexicalAnalyzer& lex, bool is_important = false);
    void may_be_semicolon(LexicalAnalyzer& lex);

    /**
    * <assignment-expression> ::= <logical-or-expression>{("=" | "+=" | "-=" | "*=" | "/=" | "%=" | ">>=" | "<<=" | "&=" | "^=" | "|=" | "->=" | "~=")<logical-or-expression>}
    */
    void assignment_expression(LexicalAnalyzer& lex);
    /**
    * <logical-or-expression> ::= <logical-and-expression>{"||" <logical-and-expression>}
    */
    void logical_or_expression(LexicalAnalyzer& lex);
    /**
    * <logical-and-expression> ::= <bitwise-or-expression>{"&&" <bitwise-or-expression>}
    */
    void logical_and_expression(LexicalAnalyzer& lex);
    /**
    * <bitwise-or-expression> ::= <bitwise-xor-expression>{"|" <bitwise-xor-expression>}
    */
    void bitwise_or_expression(LexicalAnalyzer& lex);
    /**
    * <bitwise-xor-expression> ::= <bitwise-consequence-expression>{"^" <bitwise-consequence-expression>}
    */
    void bitwise_xor_expression(LexicalAnalyzer& lex);
    /**
    * <bitwise-consequence-expression> ::= <bitwise-and-expression>{"->" <bitwise-and-expression>}
    */
    void bitwise_consequence_expression(LexicalAnalyzer& lex);
    /**
    * <bitwise-and-expression> ::= <equality-expression>{"&" <equality-expression>}
    */
    void bitwise_and_expression(LexicalAnalyzer& lex);
    /**
    * <equality-expression> ::= <comparison-expression>{("!=" | "==") <comparison-expression>}
    */
    void equality_expression(LexicalAnalyzer& lex);
    /**
    * <comparison-expression> ::= <bitwise-shift-expression>{(">" | ">=" | "<" | "<=") <bitwise-shift-expression>}
    */
    void comparison_expression(LexicalAnalyzer& lex);
    /**
    * <bitwise-shift-expression> ::= <plus-expression>{(">>" | "<<") <plus-expression>}
    */
    void bitwise_shift_expression(LexicalAnalyzer& lex);
    /**
    * <plus-expression> ::= <multiply-expression>{("+" | "-") <multiply-expression>}
    */
    void plus_expression(LexicalAnalyzer& lex);
    /**
    * <multiply-expression> ::= <unary-expression>{("*" | "/" | "%") <unary-expression>}
    */
    void multiply_expression(LexicalAnalyzer& lex);
    /**
    * <unary-expression> ::= <construct-expression>{("+" | "-" | "!" | "~" | "++" | "--") <construct-expression>}
    */
    void unary_expression(LexicalAnalyzer& lex);
    /**
    * <construct-expression> ::= <identifier> | <literal> | <field> | "("<expression>")"
    */
    void construct_expression(LexicalAnalyzer& lex);
    /**
    * <field> ::= <identifier>{("."<identifier> | "["<expression>"]" | "("[<expression>{","<expression>}]")")}
    */
    void field(LexicalAnalyzer& lex, bool only_array = false);

    /**
    * <if-statement> ::= "if("<expression>")" <statement> {"elif("<expression")" <statement>} ["else" <statement>]
    */
    void if_statement(LexicalAnalyzer& lex);
    /**
    * <goto-statement> ::= ("goto" <identifier> | "break" | "continue")";"
    */
    void goto_statement(LexicalAnalyzer& lex);
    /**
    * <return-statement> ::= "return" [<expression>]";"
    */
    void return_statement(LexicalAnalyzer& lex);
    /**
    * <while-statement> ::= "while" "("<expression>")" (<statement> | ";")
    */
    void while_statement(LexicalAnalyzer& lex);
    /**
    * <for-statement> ::= "for" "("[<expression>]";"[<expression>]";"[<expression>]")"<statement>
    */
    void for_statement(LexicalAnalyzer& lex);
    /**
    * <switch-statement> ::= "switch" "("<expression>"){"[<switch-block>]["default" ":"<statement>]"}"
    * 
    * <switch-block> ::= "case" <expression>":"<statement>
    */
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