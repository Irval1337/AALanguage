#pragma once

#include <stack>
#include "TableIdentifiers.h"
#include "LexicalAnalyzer.h"

class Semantic {
public:
    ExprType string_to_type(std::string str);
    bool is_convertible(Type first, Type second, bool is_func = false);
    Type calc_expr_type(Type first, Type second);
    std::string type_to_string(Type type);
    void add_identifier(Identifier* ident, bool is_global = false);
    Identifier* get_identifier(std::string name);
    void del_table();
    void create_table(bool new_func = false);
    void create_function(std::string name, Type type, int pref, std::vector<int> inits, std::vector<Identifier*> ptrs, int ptr);
    Function get_function(std::string name, std::vector<Type> params);
    void init_array(ExprType type, int size, void*& value, std::string name);
    bool check_operation(Type f, Type s, std::string operation);
    Type literal_to_type(LexicalAnalyzer& lex, Token current_token);

    std::stack<std::pair<Type, bool>> exprs;
    std::stack<int*> break_ptr, continue_ptr;
    std::vector<Identifier*> params_ptrs;
    int function_params_pref = 0;
    bool is_in_function_header = false, was_function_assign = false;
    std::vector<int> function_params_init;
    std::unordered_map<Function, TableIdentifiers*, FunctionHasher> funcs;
    Type current_function = Type();
private:
    TableIdentifiers* tid = new TableIdentifiers();
    TableIdentifiers* global_tid = new TableIdentifiers();
};