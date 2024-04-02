#pragma once

#include <vector>
#include <stack>
#include "TableIdentifiers.h"
#include <iostream>
#include "Large.h"
#include <chrono>
#include <random>

//! A helper class for generating and executing the internal representation of the program.
class Poliz {
public:
    //! Enumeration of all possible expression types in POLIZ.
    enum PolizType {
        GO, FGO, TGO, LABEL, ADDRESS, POINTER, GETARR, CALL, BLANK, SEMICOLON, LITERAL, USING, COMMA, ASSIGN, LOGICAL_OR,
        LOGICAL_AND, BITWISE_OR, BITWISE_XOR, BITWISE_CONS, BITWISE_AND, EQUALITY, CMP, BITWISE_SHIFT, PLUS, MULT, UNARY,
        SWITCH_CMP, SWITCH_POP, RET, BOOL_LITERAL, BYTE_LITERAL, CHAR_LITERAL, DOUBLE_LITERAL, FLOAT_LITERAL, INT_LITERAL,
        LONG_LITERAL, SHORT_LITERAL, STRING_LITERAL, UINT_LITERAL, ULONG_LITERAL, LARGE_LITERAL, STACK_PLUG, READ, PRINT, READLN, NORET,
        GETSTR, EXIT, CONVERT, RAND, STRLEN, TIME
    };

    //! Default constructor.
    Poliz() {}
    /**
    * @brief Constructor passing a pointer to the current function in semantic analysis.
    *
    * @param current_function Pointer to the Type of the current function
    */
    Poliz(Type* current_function);
    //! Destructor. Frees all dynamic memory of the internal representation.
    ~Poliz();
    /**
    * @brief Adds a new token - value pair (POLIZ expression type, pointer to value).
    *
    * @param l The value of the new token
    */
    void put_lex(std::pair<PolizType, void*> l);
    //! Adds a new blank token.
    void blank();
    /**
    * @brief Gets the current number of tokens in POLIZ.
    *
    * @return Gets the current number of tokens in the internal representation.
    */
    int get_size();
    /**
    * @brief Converts an internal type 1 value to a type 2 value.
    * 
    * @param op Token whose value will be converted
    * @param to Final value type
    *
    * @return A void* pointer to the converted value.
    */
    void* convert(std::pair<PolizType, void*> op, PolizType to);
    /**
    * @brief Performs transformations of two POLYZ tokens, bringing their values to the same type.
    *
    * The conversion is performed using extensional transformations based on "prioritization" of types.
    * 
    * @param op1 First token operand
    * @param op2 Second token operand
    *
    * @return A pair (type, (op1, op2)) where type is the type of both operands, op1 and op2 are the original values converted to that type.
    */
    std::pair<PolizType, std::pair<void*, void*>> make_same(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2);
    /**
    * @brief Converts a token of type ADDRESS to a token with a pointer to its value.
    *
    * Gets the memory value reserved in the current Identifier of a token of type ADDRESS.
    *
    * @param ptr A pointer that is part of a token of type ADDRESS.
    *
    * @return Token with the type corresponding to the current value of the variable.
    */
    std::pair<PolizType, void*> address_to_value(void* ptr);
    /**
    * @brief Copies and returns the current value of the variable.
    *
    * @param ptr A pointer that is part of a token of type ADDRESS.
    *
    * @return Pointer to the copied value.
    */
    void* copy_value(void* ptr);
    /**
    * @brief Converts ExprType to a valid PolizType.
    *
    * @param type Source type of expression
    *
    * @return Converted type that is part of PolizType.
    */
    PolizType type_to_poliz(ExprType type);
    /**
    * @brief Performs a binary operation and returns its result.
    *
    * Converts the input operands to a common type and performs the desired operation.
    * 
    * @param op1 First operand,
    * @param op2 Second operand
    * @param operation Operation string
    *
    * @return A POLIZ token with the result of the operation.
    */
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2, std::string operation);
    /**
    * @brief Performs an unary operation and returns its result.
    * 
    * @param op Operand,
    * @param operation Operation string
    *
    * @return A POLIZ token with the result of the operation.
    */
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op, std::string operation);
    /**
    * @brief Executes a function call subject to stack constraint (recursion depth 5*10^5).
    * 
    * The CALL token is used to determine how many parameters will have the specified value and how many will receive the default value. For this prefix, sets the values in the TID of the function and changes the pointer to the current instruction.
    *
    * @param st Reference to the stack of expression calculation results,
    * @param p Reference to an integer offset pointer in internal representation
    */
    void call_function(std::stack<std::pair<PolizType, void*>>& st, int& p);
    /**
    * @brief The main function that performs a walkthrough of the POLIZ token array.
    * 
    * Each instruction is analyzed according to its POLIZ type.
    *
    * @param entry_func The function that is the entry point of the program. By default it has the name main.
    *
    * @return POLIZ token, which is the result of the program operation - the return value of main or the call to exit().
    */
    std::pair<PolizType, void*> execute(Function entry_func);
    /**
    * @brief Helper operator to get a token by its number in the POLIZ arrayþ
    *
    * @param ind Token index
    *
    * @return POLIZ token on the specified index.
    */
    std::pair<PolizType, void*>& operator[](int ind) {
        return lexes[ind];
    }
private:
    std::vector<std::pair<PolizType, void*>> lexes;
    int literal_prior[12] = { 0, 2, 1, 11, 7, 5, 9, 3, 0, 6, 10, 12 };
    std::stack<std::pair<int, PolizType>> func_calls;
    std::vector<std::pair<PolizType, void*>> global_lexes;
    std::stack<std::vector<std::pair<Identifier*, void*>>> tid_vals;
    std::mt19937 rnd;
    Type* current_function;

    ExprType string_to_type(std::string str);
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    void add_tid_values(TableIdentifiers* current_tid, bool is_first = true);
    void clear_tid_values(TableIdentifiers* current_tid);
    void restore_tid_values();
};