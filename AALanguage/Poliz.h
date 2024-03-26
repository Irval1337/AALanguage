#pragma once

#include <vector>
#include <stack>
#include "TableIdentifiers.h"
#include <iostream>
#include "Large.h"
#include <chrono>
#include <random>

class Poliz {
public:
    enum PolizType {
        GO, FGO, TGO, LABEL, ADDRESS, POINTER, GETARR, CALL, BLANK, SEMICOLON, LITERAL, USING, COMMA, ASSIGN, LOGICAL_OR,
        LOGICAL_AND, BITWISE_OR, BITWISE_XOR, BITWISE_CONS, BITWISE_AND, EQUALITY, CMP, BITWISE_SHIFT, PLUS, MULT, UNARY,
        SWITCH_CMP, SWITCH_POP, RET, BOOL_LITERAL, BYTE_LITERAL, CHAR_LITERAL, DOUBLE_LITERAL, FLOAT_LITERAL, INT_LITERAL,
        LONG_LITERAL, SHORT_LITERAL, STRING_LITERAL, UINT_LITERAL, ULONG_LITERAL, LARGE_LITERAL, STACK_PLUG, READ, PRINT, READLN, NORET,
        GETSTR, EXIT, CONVERT, RAND, STRLEN, TIME
    };

    Poliz() {}
    Poliz(Type* current_function);
    ~Poliz();
    void put_lex(std::pair<PolizType, void*> l);
    void blank();
    int get_size();
    void print();
    void* convert(std::pair<PolizType, void*> op, PolizType to);
    std::pair<PolizType, std::pair<void*, void*>> make_same(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2);
    std::pair<PolizType, void*> address_to_value(void* ptr);
    PolizType type_to_poliz(ExprType type);
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2, std::string operation);
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op, std::string operation);
    void call_function(std::stack<std::pair<PolizType, void*>>& st, int& p);
    std::pair<PolizType, void*> execute(Function entry_func);
    std::pair<PolizType, void*>& operator[](int ind) {
        return lexes[ind];
    }
private:
    std::vector<std::pair<PolizType, void*>> lexes;
    int literal_prior[12] = { 0, 2, 1, 11, 7, 5, 9, 3, 0, 6, 10, 12 };
    std::stack<std::pair<int, PolizType>> func_calls;
    std::vector<std::pair<PolizType, void*>> global_lexes;
    std::mt19937 rnd;
    Type* current_function;

    ExprType string_to_type(std::string str);
};