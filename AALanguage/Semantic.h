#pragma once

#include <stack>
#include "TableIdentifiers.h"
#include "LexicalAnalyzer.h"

//! A helper class that performs basic actions on the semantics of the language.
class Semantic {
public:
    /**
    * @brief Translating a string with a data type name into ExprType.
    *
    * @param str String containing data type
    * 
    * @return ExprType corresponding to the internal representation.
    */
    ExprType string_to_type(std::string str);
    /**
    * @brief Checks if implicit type conversion is possible.
    *
    * @param first The first type
    * @param second The second type
    * @param is_func Flag indicating that a type conversion of the function arguments is in progress.
    *
    * @return Whether implicit type conversion is possible.
    */
    bool is_convertible(Type first, Type second, bool is_func = false);
    /**
    * @brief Calculates the result of a conversion of two types.
    *
    * @param first The first type
    * @param second The second type
    *
    * @return The resultant type to which both data types are cast.
    */
    Type calc_expr_type(Type first, Type second);
    /**
    * @brief Translates the Type to a string.
    *
    * @param type Type for conversion
    *
    * @return String corresponding to the given type taking into account arrays and their size.
    */
    std::string type_to_string(Type type);
    /**
    * @brief Adds a new identifier to the current table.
    * 
    * Checks that there is no identifier with the same name in the current scope and adds it.
    *
    * @param ident Pointer to the new identifier
    * @param is_global Whether the new identifier is global
    */
    void add_identifier(Identifier* ident, bool is_global = false);
    /**
    * @brief Gets a pointer to an identifier by its name.
    *
    * Checks if an identifier with the specified name exists in the current scope and returns a pointer to its instance of the Identifier class.
    * The search is performed by traversing from the current TID through the parent tables.
    *
    * @param name Identifier name
    * 
    * @return Pointer to the required identifier.
    */
    Identifier* get_identifier(std::string name);
    /**
    * @brief Deletes the current TID and returns to the parent table.
    */
    void del_table();
    /**
    * @brief Creates a new TID and adds it as a child of the current one.
    * 
    * @param new_func Whether the current variable is part of the function arguments
    */
    void create_table(bool new_func = false);
    /**
    * @brief Creates a new function and adds it to the TID of functions.
    *
    * @param name Function name
    * @param type Returnable type
    * @param pref The length of the prefix with no default value
    * @param inits Vector-array with pointers to initialize variables in internal representation
    * @param ptrs Vector-array with pointers to all argument identifiers
    * @param ptr Function offset in internal representation
    */
    void create_function(std::string name, Type type, int pref, std::vector<int> inits, std::vector<Identifier*> ptrs, int ptr);
    /**
    * @brief Searches for a function with the specified name and set of argument types.
    *
    * Searches the function first by prefixing argument types without a default value, and then by all values. If the function call is ambiguous, it throws an exception.
    * 
    * @param name Function name
    * @param params Vector-array of standard argument types
    * 
    * @return The function found or throws an exception.
    */
    Function get_function(std::string name, std::vector<Type> params);
    /**
    * @brief Initializes array memory with empty pointers.
    * 
    * @param type Type of array elements
    * @param size Array size
    * @param value Reference to the array identifier value pointer
    * @param name Array name
    */
    void init_array(ExprType type, int size, void*& value, std::string name);
    /**
    * @brief Checks whether the operation is applicable to two operand types.
    *
    * @param f First operand
    * @param s Second operand
    * @param operation String of operation
    * 
    * @return Whether the operation is applicable to two operand types.
    */
    bool check_operation(Type f, Type s, std::string operation);
    /**
    * @brief Converts a string containing a literal to a Type.
    *
    * @param lex Reference to the lexical analyzer used
    * @param current_token Convertible lexeme
    *
    * @return The type of given literal.
    */
    Type literal_to_type(LexicalAnalyzer& lex, Token current_token);

    //! A stack of current type expressions. Consists of pairs (Type, bool) = (Type of expression, whether lvalue).
    std::stack<std::pair<Type, bool>> exprs;
    //! A stack of pointers to the lowest level of the break operator in the internal representation.
    std::stack<int*> break_ptr;
    //! A stack of pointers to the lowest level of the continue operator in the internal representation.
    std::stack<int*> continue_ptr;
    //! Vector-array of pointers to the current set of function parameters.
    std::vector<Identifier*> params_ptrs;
    //! The length of the current prefix with arguments with no default value in the function under consideration.
    int function_params_pref = 0;
    //! Flag to determine if you are inside the parameters of the function.
    bool is_in_function_header = false;
    //! Flag to determine if the parameters have started with the default value.
    bool was_function_assign = false;
    //! Vector-array with pointers to the offset of initialization of parameters of the current function in the internal representation of the program.
    std::vector<int> function_params_init;
    //! Function hash table.
    std::unordered_map<Function, TableIdentifiers*, FunctionHasher> funcs;
    //! Type of current function.
    Type current_function = Type();
private:
    TableIdentifiers* tid = new TableIdentifiers();
    TableIdentifiers* global_tid = new TableIdentifiers();
};