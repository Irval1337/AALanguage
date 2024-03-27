#include "Analyzer.h"

Analyzer::Analyzer() {
    prog = Poliz(&semantic.current_function);
}

void Analyzer::execute(std::string path, std::string lib_path) {
    bool started = false;
    try {
        LibParser lib_parser(lib_path);
        LexicalAnalyzer lex(path, lib_parser);

        printf("\x1B[32mThe lexical analysis was successful\n\033[0m");

        current_token = lex.get_token();
        program(lex);

        if (current_token.line != -1) {
            printf("\x1B[31mUnexpected end of file\n\033[0m");
            exit(0);
        }

        printf("\x1B[32mThe syntactic & semantic analysis was successful\n\033[0m");
        printf("\x1B[32mThe internal representation of the program has been successfully generated\n\033[0m");
        auto main = semantic.get_function("main", std::vector<Type>());
        started = true;
        auto result = prog.execute(main);
        std::cout << "\nExit code: " << (result.second == nullptr ? "unknown" : *(std::string*)prog.convert(result, Poliz::PolizType::STRING_LITERAL));
    } catch (std::exception ex) {
        printf(("\x1B[31m\n" + std::string(ex.what())).c_str());
        if (current_token.line != -1) {
            std::cout << " (" << current_token.line << " line)";
        }
        printf("\033[0m");
        if (started)
            std::cout << "\nExit code: -1";
    }
}

void Analyzer::semicolon(LexicalAnalyzer& lex, bool is_important) {
    if (current_token.line == -1)
        throw std::exception("Invalid end of line: ';' expected");
    if (current_token.value != ";")
        throw std::exception("Invalid token: ';' expected");
    prog.put_lex({ Poliz::PolizType::SEMICOLON, new std::string(";") });
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

void Analyzer::may_be_semicolon(LexicalAnalyzer& lex) {
    while (current_token.value == ";") {
        prog.put_lex({ Poliz::PolizType::SEMICOLON, new std::string(";") });
        current_token = lex.get_token();
    }
}

void Analyzer::program(LexicalAnalyzer& lex) {
    may_be_semicolon(lex);
    preprocessor(lex);
    if (current_token.line == -1) return;
    program_body(lex);
}

void Analyzer::preprocessor(LexicalAnalyzer& lex) {
    while (current_token.value == "using") {
        current_token = lex.get_token();
        namespace_definition(lex);
        semicolon(lex);
        may_be_semicolon(lex);
    }
    may_be_semicolon(lex);
}

void Analyzer::namespace_definition(LexicalAnalyzer& lex) {
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string curr_namespace = current_token.value;
    current_token = lex.get_token();
    while (current_token.value == ".") {
        curr_namespace.push_back('.');
        current_token = lex.get_token();
        if (current_token.type != LexicalAnalyzer::token_type::identifier) {
            throw std::exception("Invalid token: identifier expected");
        }
        curr_namespace += current_token.value;
        current_token = lex.get_token();
    }
    prog.put_lex({ Poliz::PolizType::USING, new std::string(curr_namespace) });
}

void Analyzer::program_body(LexicalAnalyzer& lex) {
    if (current_token.line == -1) return;
    function_or_var_definitions(lex, true);
    may_be_semicolon(lex);
    program_body(lex);
}

void Analyzer::var_definition(LexicalAnalyzer& lex) {
    bool is_const = current_token.value == "const";
    ExprType curr_type;
    if (is_const) {
        current_token = lex.get_token();
        curr_type = semantic.string_to_type(current_token.value);
        current_token = lex.prev_token();
    } else
        curr_type = semantic.string_to_type(current_token.value);
    type(lex);
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    Identifier* ident = nullptr;
    bool is_array = current_token.value == "[";
    int size = 0;
    if (is_array) {
        current_token = lex.get_token();
        if (lex.is_literal(current_token.value) != lex.integer)
            throw std::exception("Invalid token: integer literal expected");
        size = std::stoi(current_token.value);
        current_token = lex.get_token();
        if (current_token.value != "]")
            throw std::exception("Invalid token: ']' expected");
        current_token = lex.get_token();

        ident = new Identifier(name, Type(curr_type, is_const, true, size));
    } else {
        ident = new Identifier(name, Type(curr_type, is_const, false));
    }
    if (current_token.value == "=") {
        current_token = lex.get_token();
        prog.put_lex({ Poliz::PolizType::ADDRESS, ident });
        expression(lex, true);
        if (!semantic.is_convertible(semantic.exprs.top().first, ident->type))
            throw std::exception(("Cannot convert " + semantic.type_to_string(semantic.exprs.top().first) + " to " + semantic.type_to_string(ident->type)).c_str());
        semantic.exprs.pop();
        prog.put_lex({ Poliz::PolizType::ASSIGN, new std::string("=") });

        if (semantic.is_in_function_header) {
            if (!semantic.was_function_assign)
                semantic.was_function_assign = true;
        }
    } else if (semantic.is_in_function_header) {
        if (semantic.was_function_assign)
            throw std::exception("Parameters with default values cannot be followed by standard ones");
        ++semantic.function_params_pref;
    }
    if (current_token.value != "=") {
        if (is_array) {
            semantic.init_array(curr_type, size, ident->value, ident->name);
        } else ident->value = nullptr;
    }
    semantic.add_identifier(ident);
}

void Analyzer::var_definitions(LexicalAnalyzer& lex, bool need_semicolon, bool is_program) {
    bool is_const = current_token.value == "const";
    ExprType curr_type;
    if (is_const) {
        current_token = lex.get_token();
        curr_type = semantic.string_to_type(current_token.value);
        current_token = lex.prev_token();
    } else
        curr_type = semantic.string_to_type(current_token.value);
    type(lex);
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    Identifier* ident = nullptr;
    bool is_array = current_token.value == "[";
    int size = 0;
    if (is_array) {
        current_token = lex.get_token();
        if (lex.is_literal(current_token.value) != lex.integer)
            throw std::exception("Invalid token: integer literal expected");
        size = std::stoi(current_token.value);
        current_token = lex.get_token();
        if (current_token.value != "]")
            throw std::exception("Invalid token: ']' expected");
        current_token = lex.get_token();

        ident = new Identifier(name, Type(curr_type, is_const, true, size));
    } else {
        ident = new Identifier(name, Type(curr_type, is_const, false));
    }
    if (current_token.value == "=") {
        current_token = lex.get_token();
        prog.put_lex({ Poliz::PolizType::ADDRESS, ident });
        expression(lex, true);
        prog.put_lex({ Poliz::PolizType::ASSIGN, new std::string("=") });

        if (!semantic.is_convertible(semantic.exprs.top().first, ident->type))
            throw std::exception(("Cannot convert " + semantic.type_to_string(semantic.exprs.top().first) + " to " + semantic.type_to_string(ident->type)).c_str());
        semantic.exprs.pop();
    } else {
        if (is_array) semantic.init_array(curr_type, size, ident->value, ident->name);
        else ident->value = nullptr;
    }
    semantic.add_identifier(ident, is_program);
    while (current_token.value == ",") {
        current_token = lex.get_token();
        if (current_token.type != LexicalAnalyzer::token_type::identifier)
            throw std::exception("Invalid token: identifier expected");
        name = current_token.value;
        current_token = lex.get_token();
        is_array = current_token.value == "[";
        if (current_token.value == "[") {
            current_token = lex.get_token();
            if (lex.is_literal(current_token.value) != lex.integer)
                throw std::exception("Invalid token: integer literal expected");
            size = std::stoi(current_token.value);
            current_token = lex.get_token();
            if (current_token.value != "]")
                throw std::exception("Invalid token: ']' expected");
            current_token = lex.get_token();

            ident = new Identifier(name, Type(curr_type, is_const, true, size));
        } else {
            ident = new Identifier(name, Type(curr_type, is_const, false));
        }
        if (current_token.value == "=") {
            current_token = lex.get_token();
            prog.put_lex({ Poliz::PolizType::ADDRESS, ident });
            expression(lex, true);
            prog.put_lex({ Poliz::PolizType::ASSIGN, new std::string("=") });

            if (!semantic.is_convertible(semantic.exprs.top().first, ident->type))
                throw std::exception(("Cannot convert " + semantic.type_to_string(semantic.exprs.top().first) + " to " + semantic.type_to_string(ident->type)).c_str());
            semantic.exprs.pop();
        } else {
            if (is_array) semantic.init_array(curr_type, size, ident->value, ident->name);
            else ident->value = nullptr;
        }
        semantic.add_identifier(ident, is_program);
    }
    if (need_semicolon) {
        semicolon(lex);
        may_be_semicolon(lex);
    }
}

void Analyzer::type(LexicalAnalyzer& lex) {
    if (current_token.value == "const")
        current_token = lex.get_token();
    default_type(lex);
}

void Analyzer::default_type(LexicalAnalyzer& lex) {
    for (int i = 0; i < service_types.size(); ++i) {
        if (service_types[i] == current_token.value) {
            current_token = lex.get_token();
            return;
        }
    }
    throw std::exception("Invalid token: service type expected");
}

void Analyzer::function(LexicalAnalyzer& lex, bool is_struct) {
    ExprType curr_type = semantic.string_to_type(current_token.value);
    if (current_token.value == "void")
        current_token = lex.get_token();
    else
        default_type(lex);

    semantic.function_params_pref = 0;
    Type func_type;
    func_type.expr_type = curr_type;
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    std::unordered_map<std::string, void*> values;

    semantic.current_function = func_type;

    if (current_token.value == "[" && curr_type != ExprType::Void) {
        current_token = lex.get_token();
        if (lex.is_literal(current_token.value) != lex.integer)
            throw std::exception("Invalid token: integer literal expected");
        int size = std::stoi(current_token.value);
        current_token = lex.get_token();
        if (current_token.value != "]")
            throw std::exception("Invalid token: ']' expected");
        current_token = lex.get_token();

        func_type.is_array = true;
        func_type.array_size = size;
    }

    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();

    semantic.create_table(true);
    bool was_parameters = false;
    int ptr = prog.get_size();
    prog.put_lex({ Poliz::PolizType::LABEL, nullptr });

    semantic.function_params_init.clear();
    semantic.params_ptrs.clear();
check_function_brace:
    if (current_token.value == ")" || was_parameters) {
        semantic.create_function(name, func_type, semantic.function_params_pref, semantic.function_params_init, semantic.params_ptrs, ptr);
        semantic.params_ptrs.clear();
        current_token = lex.get_token();
        if (current_token.value != "{")
            throw std::exception("Invalid token: '{' expected");
        current_token = lex.get_token();
        while (current_token.value != "}" && current_token.line != -1)
            statement(lex);
        if (current_token.value != "}")
            throw std::exception("Invalid token: '}' expected");
        current_token = lex.get_token();
        if (func_type.expr_type == ExprType::Void)
            prog.put_lex({ Poliz::PolizType::NORET, nullptr });
        semantic.current_function = Type();
        may_be_semicolon(lex);
    } else {
        parameter_list(lex);
        was_parameters = true;
        goto check_function_brace;
    }
}

void Analyzer::function_or_var_definitions(LexicalAnalyzer& lex, bool is_program) {
    if (current_token.value == "const") {
        var_definitions(lex, true, is_program);
        return;
    }
    if (current_token.value == "void") {
        function(lex);
        return;
    }
    default_type(lex);
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    current_token = lex.get_token();
    std::string str = current_token.value;
    if (current_token.value == "[") {
        current_token = lex.get_token();
        current_token = lex.get_token();
        current_token = lex.get_token();
        str = current_token.value;
        current_token = lex.prev_token();
        current_token = lex.prev_token();
        current_token = lex.prev_token();
    }
    if (str == "(") {
        current_token = lex.prev_token();
        current_token = lex.prev_token();
        function(lex);
    } else if (str == "=" || str == "," || str == ";") {
        current_token = lex.prev_token();
        current_token = lex.prev_token();
        var_definitions(lex, true, is_program);
    } else
        throw std::exception("Invalid token: '=', ',', ';' or '(' expected");
}

void Analyzer::expression(LexicalAnalyzer& lex, bool is_vars, bool is_print) {
    assignment_expression(lex);
    if (is_print)
        prog.put_lex({ Poliz::PolizType::PRINT, nullptr });

    while (!is_vars && current_token.value == ",") {
        semantic.exprs.pop();
        current_token = lex.get_token();
        assignment_expression(lex);
        if (is_print)
            prog.put_lex({ Poliz::PolizType::PRINT, nullptr });
        if (!is_print)
            prog.put_lex({ Poliz::PolizType::COMMA, new std::string(",") });
    }
}

void Analyzer::assignment_expression(LexicalAnalyzer& lex) {
    logical_or_expression(lex);
    std::vector<std::string> operations;
    while (current_token.value == "=" || current_token.value == "+=" || current_token.value == "-=" || current_token.value == "*=" ||
        current_token.value == "/=" || current_token.value == "%=" || current_token.value == ">>=" || current_token.value == "<<=" ||
        current_token.value == "&=" || current_token.value == "^=" || current_token.value == "|=" || current_token.value == "->=" ||
        current_token.value == "~=") {

        auto operation = current_token.value;
        auto full_operation = operation;
        auto tmp_operation = operation;
        if (operation != "=")
            operation = operation.substr(0, operation.size() - 1);
        if (!semantic.exprs.top().second)
            throw std::exception("Cannot apply assignment operator to rvalue");
        if (semantic.exprs.top().first.is_const)
            throw std::exception("Cannot apply assignment operator to const");
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        current_token = lex.get_token();
        logical_or_expression(lex);
        operations.push_back(full_operation);
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + tmp_operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), curr.second });
    }
    for (int i = (int)operations.size() - 1; i >= 0; --i) {
        prog.put_lex({ Poliz::PolizType::ASSIGN, new std::string(operations[i]) });
    }
}

void Analyzer::logical_or_expression(LexicalAnalyzer& lex) {
    logical_and_expression(lex);
    while (current_token.value == "||") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        logical_and_expression(lex);
        prog.put_lex({ Poliz::PolizType::LOGICAL_OR, new std::string("||") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "||"))
            throw std::exception(("Cannot apply || operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void Analyzer::logical_and_expression(LexicalAnalyzer& lex) {
    bitwise_or_expression(lex);
    while (current_token.value == "&&") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        bitwise_or_expression(lex);
        prog.put_lex({ Poliz::PolizType::LOGICAL_AND, new std::string("&&") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "&&"))
            throw std::exception(("Cannot apply && operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void Analyzer::bitwise_or_expression(LexicalAnalyzer& lex) {
    bitwise_xor_expression(lex);
    while (current_token.value == "|") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        bitwise_xor_expression(lex);
        prog.put_lex({ Poliz::PolizType::BITWISE_OR, new std::string("|") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "|"))
            throw std::exception(("Cannot apply | operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::bitwise_xor_expression(LexicalAnalyzer& lex) {
    bitwise_consequence_expression(lex);
    while (current_token.value == "^") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        bitwise_consequence_expression(lex);
        prog.put_lex({ Poliz::PolizType::BITWISE_XOR, new std::string("^") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "^"))
            throw std::exception(("Cannot apply ^ operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::bitwise_consequence_expression(LexicalAnalyzer& lex) {
    bitwise_and_expression(lex);
    while (current_token.value == "->") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        bitwise_and_expression(lex);
        prog.put_lex({ Poliz::PolizType::BITWISE_CONS, new std::string("->") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "->"))
            throw std::exception(("Cannot apply -> operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::bitwise_and_expression(LexicalAnalyzer& lex) {
    equality_expression(lex);
    while (current_token.value == "&") {
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        equality_expression(lex);
        prog.put_lex({ Poliz::PolizType::BITWISE_AND, new std::string("&") });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, "&"))
            throw std::exception(("Cannot apply & operator with " + semantic.type_to_string(prev) + " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::equality_expression(LexicalAnalyzer& lex) {
    comparison_expression(lex);
    while (current_token.value == "==" || current_token.value == "!=") {
        auto operation = current_token.value;
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        comparison_expression(lex);
        prog.put_lex({ Poliz::PolizType::EQUALITY, new std::string(operation) });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void Analyzer::comparison_expression(LexicalAnalyzer& lex) {
    bitwise_shift_expression(lex);
    while (current_token.value == ">" || current_token.value == "<" || current_token.value == ">=" || current_token.value == "<=") {
        auto operation = current_token.value;
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        bitwise_shift_expression(lex);
        prog.put_lex({ Poliz::PolizType::CMP, new std::string(operation) });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void Analyzer::bitwise_shift_expression(LexicalAnalyzer& lex) {
    plus_expression(lex);
    while (current_token.value == ">>" || current_token.value == "<<") {
        auto operation = current_token.value;
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        plus_expression(lex);
        prog.put_lex({ Poliz::PolizType::BITWISE_SHIFT, new std::string(operation) });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::plus_expression(LexicalAnalyzer& lex) {
    multiply_expression(lex);
    while (current_token.value == "+" || current_token.value == "-") {
        auto operation = current_token.value;
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        multiply_expression(lex);
        prog.put_lex({ Poliz::PolizType::PLUS, new std::string(operation) });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::multiply_expression(LexicalAnalyzer& lex) {
    unary_expression(lex);
    while (current_token.value == "*" || current_token.value == "/" || current_token.value == "%") {
        auto operation = current_token.value;
        current_token = lex.get_token();
        auto prev = semantic.exprs.top().first;
        semantic.exprs.pop();
        unary_expression(lex);
        prog.put_lex({ Poliz::PolizType::MULT, new std::string(operation) });
        if (!semantic.check_operation(prev, semantic.exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + semantic.type_to_string(prev) +
                " and " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        auto curr = semantic.exprs.top();
        semantic.exprs.pop();
        semantic.exprs.push({ semantic.calc_expr_type(prev, curr.first), false });
    }
}

void Analyzer::unary_expression(LexicalAnalyzer& lex) {
    std::vector<std::string> operations;
    while (current_token.value == "+" || current_token.value == "-" || current_token.value == "!" || current_token.value == "~" ||
        current_token.value == "++" || current_token.value == "--") {
        operations.push_back(current_token.value);
        current_token = lex.get_token();
    }
    construct_expression(lex);
    for (int i = (int)operations.size() - 1; i >= 0; --i) {
        prog.put_lex({ Poliz::PolizType::UNARY, new std::string(operations[i]) });
    }

    auto curr = semantic.exprs.top();
    semantic.exprs.pop();
    for (int i = operations.size() - 1; i >= 0; --i) {
        if (curr.first.is_array)
            throw std::exception(("Cannot apply " + operations[i] + " operator to " + semantic.type_to_string(curr.first)).c_str());
        if (operations[i] == "+" || operations[i] == "-" || operations[i] == "~") {
            if (curr.first.expr_type == ExprType::String || curr.first.expr_type == ExprType::Bool || curr.first.expr_type == ExprType::Unknown ||
                curr.first.expr_type == ExprType::Void)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + semantic.type_to_string(curr.first)).c_str());
            curr.second = false;
        } else if (operations[i] == "!") {
            if (curr.first.expr_type != ExprType::Bool)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + semantic.type_to_string(curr.first)).c_str());
            curr.second = false;
        } else {
            if (curr.first.expr_type == ExprType::String || curr.first.expr_type == ExprType::Bool || curr.first.expr_type == ExprType::Unknown ||
                curr.first.expr_type == ExprType::Void)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + semantic.type_to_string(curr.first)).c_str());
            if (!curr.second)
                throw std::exception(("Cannot apply " + operations[i] + " operator to rvalue").c_str());
            if (curr.first.is_const)
                throw std::exception(("Cannot apply " + operations[i] + " operator to const").c_str());
        }
    }
    semantic.exprs.push(curr);
}

void Analyzer::construct_expression(LexicalAnalyzer& lex) {
    if (current_token.type == LexicalAnalyzer::token_type::literal) {
        semantic.exprs.push({ semantic.literal_to_type(lex, current_token), false });
        prog.put_lex({ Poliz::PolizType::LITERAL, new std::pair<std::string, Type>(current_token.value, semantic.literal_to_type(lex, current_token)) });
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

void Analyzer::field(LexicalAnalyzer& lex, bool only_array) {
    if (current_token.type == LexicalAnalyzer::token_type::service) {
        if (current_token.value == "convert") {
            convert_statement(lex);
            if (current_token.value == "[")
                throw std::exception("Cannot apply operator [] to rvalue string");
        } else if (current_token.value == "rand")
            rand_statement(lex);
        else if (current_token.value == "strlen")
            strlen_statement(lex);
        else if (current_token.value == "time")
            time_statement(lex);
        else
            throw std::exception("Invalid token: identifier expected");
        return;
    } else if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    Identifier* ident = nullptr;
    if (current_token.value == "[")
        ident = semantic.get_identifier(name);
    while (current_token.value == "[" || current_token.value == "(" && !only_array) { // TODO: field .
        if (current_token.value == "[") {
            if (!ident->type.is_array && ident->type.expr_type != ExprType::String)
                throw std::exception("Cannot apply operator [] to anything other than an array or string");
            current_token = lex.get_token();

            prog.put_lex({ Poliz::PolizType::ADDRESS, ident });
            expression(lex, true);
            if (!semantic.is_convertible(semantic.exprs.top().first, Type(ExprType::Long, false, false)))
                throw std::exception("Cannot access index that is not an integer");
            if (ident->type.is_array)
                prog.put_lex({ Poliz::PolizType::GETARR, new std::string("[]") });
            else
                prog.put_lex({ Poliz::PolizType::GETSTR, new std::string("[]") });
            semantic.exprs.pop();
            if (current_token.value != "]")
                throw std::exception("Invalid token: ']' expected");
            current_token = lex.get_token();
            if (current_token.value != "[") {
                semantic.exprs.push({ Type(ident->type.is_array ? ident->type.expr_type : ExprType::Char, false, false), true });
            } else if (ident->type.expr_type != ExprType::String)
                throw std::exception("Cannot apply operator [] to anything other than an array or string");
            else {
                current_token = lex.get_token();
                expression(lex, true);
                prog.put_lex({ Poliz::PolizType::GETSTR, new std::string("[]") });
                if (!semantic.is_convertible(semantic.exprs.top().first, Type(ExprType::Long, false, false)))
                    throw std::exception("Cannot access index that is not an integer");
                semantic.exprs.pop();
                if (current_token.value != "]")
                    throw std::exception("Invalid token: ']' expected");
                current_token = lex.get_token();
                semantic.exprs.push({ Type(ExprType::Char, false, false), true });
            }
            return;
        } /*else if (current_token.value == ".") {
            current_token = lex.get_token();
            if (current_token.type != LexicalAnalyzer::token_type::identifier)
                throw std::exception("Invalid token: identifier expected");
            current_token = lex.get_token();
        } */ else { // TODO: check function
            current_token = lex.get_token();
            std::vector<Type> idents;
            if (current_token.value != ")") {
                expression(lex, true);
                idents.push_back(semantic.exprs.top().first);
                semantic.exprs.pop();
            }
            while (current_token.value == ",") {
                current_token = lex.get_token();
                expression(lex, true);
                idents.push_back(semantic.exprs.top().first);
                semantic.exprs.pop();
            }
            if (current_token.value != ")")
                throw std::exception("Invalid token: ')' expected");
            current_token = lex.get_token();

            auto func = semantic.get_function(name, idents);
            prog.put_lex({ Poliz::PolizType::ADDRESS, new Function(func) });
            prog.put_lex({ Poliz::PolizType::CALL, new int(idents.size()) });

            if (current_token.value == "[") {
                if (!func.type.is_array)
                    throw std::exception("Cannot apply operator [] to anything other than an array or string");
                current_token = lex.get_token();
                expression(lex, true);
                if (func.type.is_array != ExprType::String)
                    prog.put_lex({ Poliz::PolizType::GETARR, new std::string("[]") });
                else
                    prog.put_lex({ Poliz::PolizType::GETSTR, new std::string("[]") });
                if (!semantic.is_convertible(semantic.exprs.top().first, Type(ExprType::Long, false, false)))
                    throw std::exception("Cannot access index that is not an integer");
                semantic.exprs.pop();
                current_token = lex.get_token();

                if (current_token.value != "[") {
                    semantic.exprs.push({ Type(func.type.expr_type, false, false), false });
                } else if (func.type.expr_type != ExprType::String)
                    throw std::exception("Cannot apply operator [] to anything other than an array or string");
                else {
                    current_token = lex.get_token();
                    expression(lex, true);
                    prog.put_lex({ Poliz::PolizType::GETSTR, new std::string("[]") });
                    if (!semantic.is_convertible(semantic.exprs.top().first, Type(ExprType::Long, false, false)))
                        throw std::exception("Cannot access index that is not an integer");
                    semantic.exprs.pop();
                    if (current_token.value != "]")
                        throw std::exception("Invalid token: ']' expected");
                    current_token = lex.get_token();
                    semantic.exprs.push({ Type(ExprType::Char, false, false), true });
                }
            } else
                semantic.exprs.push({ func.type, false });
            return;
        }
    }

    ident = semantic.get_identifier(name);
    semantic.exprs.push({ ident->type, true });
    prog.put_lex({ Poliz::PolizType::ADDRESS, ident });
}

void Analyzer::parameter_list(LexicalAnalyzer& lex) {
    semantic.is_in_function_header = true;
    semantic.was_function_assign = false;
    semantic.function_params_pref = 0;

    var_definition(lex);
    prog.blank();
    semantic.function_params_init.push_back(prog.get_size() - 1);

    while (current_token.value == ",") {
        current_token = lex.get_token();
        var_definition(lex);
        prog.blank();
        semantic.function_params_init.push_back(prog.get_size() - 1);
    }
    semantic.is_in_function_header = false;
}

void Analyzer::if_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "if")
        throw std::exception("Invalid token: 'if' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    if (semantic.exprs.top().first.expr_type != ExprType::Bool || semantic.exprs.top().first.is_array)
        throw std::exception(("Invalid expression type. bool expected, but found " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
    semantic.exprs.pop();
    prog.put_lex({ Poliz::PolizType::POINTER, nullptr });
    int ptrF = prog.get_size() - 1;
    prog.put_lex({ Poliz::PolizType::FGO, new std::string("!F") });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    statement(lex);
    int* ptrEnd = new int(-1);
    prog.put_lex({ Poliz::PolizType::POINTER, ptrEnd });
    prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    prog.blank();
    prog[ptrF].second = new int(prog.get_size() - 1);

    while (current_token.value == "elif") {
        current_token = lex.get_token();

        if (current_token.value != "(")
            throw std::exception("Invalid token: '(' expected");
        current_token = lex.get_token();
        expression(lex);
        if (semantic.exprs.top().first.expr_type != ExprType::Bool || semantic.exprs.top().first.is_array)
            throw std::exception(("Invalid expression type. bool expected, but found " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        semantic.exprs.pop();
        prog.put_lex({ Poliz::PolizType::POINTER, nullptr });
        ptrF = prog.get_size() - 1;
        prog.put_lex({ Poliz::PolizType::FGO, new std::string("!F") });

        if (current_token.value != ")")
            throw std::exception("Invalid token: ')' expected");
        current_token = lex.get_token();
        statement(lex);

        prog.put_lex({ Poliz::PolizType::POINTER, ptrEnd });
        prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
        prog.blank();
        prog[ptrF].second = new int(prog.get_size() - 1);
    }
    if (current_token.value == "else") {
        current_token = lex.get_token();
        statement(lex);
    }

    *ptrEnd = prog.get_size() - 1;
}

void Analyzer::goto_statement(LexicalAnalyzer& lex) {
    if (current_token.value == "break") {
        if (semantic.break_ptr.empty())
            throw std::exception("It is not possible to call break in the current context");
        current_token = lex.get_token();
        prog.put_lex({ Poliz::PolizType::POINTER, semantic.break_ptr.top() });
        prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    } else if (current_token.value == "continue") {
        if (semantic.continue_ptr.empty())
            throw std::exception("It is not possible to call continue in the current context");
        current_token = lex.get_token();
        prog.put_lex({ Poliz::PolizType::POINTER, semantic.continue_ptr.top() });
        prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    } /*else if (current_token.value == "goto") { TODO: add goto
        current_token = lex.get_token();
        if (current_token.type != LexicalAnalyzer::token_type::identifier)
            throw std::exception("Invalid token: 'identifier' expected");
        current_token = lex.get_token();
    } */else
        throw std::exception("Invalid token: 'break', 'continue' or 'goto' expected");
}

void Analyzer::return_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "return")
        throw std::exception("Invalid token: 'return' expected");
    current_token = lex.get_token();
    if (current_token.value != ";") {
        expression(lex);
        auto type = semantic.exprs.top().first;
        semantic.exprs.pop();
        if (!semantic.is_convertible(type, semantic.current_function))
            throw std::exception(("Cannot cast " + semantic.type_to_string(type) + " type to " + semantic.type_to_string(semantic.current_function)).c_str());
        prog.put_lex({ Poliz::PolizType::RET, nullptr });
    } else {
        if (!semantic.is_convertible(Type(ExprType::Void, false, false), semantic.current_function))
            throw std::exception("The function requires a return value.");
        prog.put_lex({ Poliz::PolizType::NORET, nullptr });
    }
}

void Analyzer::while_statement(LexicalAnalyzer& lex) {
    prog.blank();
    int* ptrBeg = new int(prog.get_size() - 1);
    int* ptrEnd = new int(-1);
    semantic.continue_ptr.push(ptrBeg);
    semantic.break_ptr.push(ptrEnd);
    if (current_token.value != "while")
        throw std::exception("Invalid token: 'while' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    if (semantic.exprs.top().first.expr_type != ExprType::Bool || semantic.exprs.top().first.is_array)
        throw std::exception(("Invalid expression type. bool expected, but found " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
    semantic.exprs.pop();
    prog.put_lex({ Poliz::PolizType::POINTER, ptrEnd });
    prog.put_lex({ Poliz::PolizType::FGO, new std::string("!F") });

    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    statement(lex);

    prog.put_lex({ Poliz::PolizType::POINTER, ptrBeg });
    prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });

    prog.blank();
    *ptrEnd = prog.get_size() - 1;

    semantic.continue_ptr.pop();
    semantic.break_ptr.pop();
}

void Analyzer::for_statement(LexicalAnalyzer& lex) {
    semantic.create_table();
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
        } else if (current_token.value == "const")
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
                    semantic.exprs.pop();
                }
            }
        }
    }
    semicolon(lex, true);

    prog.blank();
    int* ptrExpr = new int(prog.get_size() - 1);
    int* ptrEnd = new int(-1);
    int* ptrAct = new int(-1);
    int* ptrBody = new int(-1);
    semantic.break_ptr.push(ptrEnd);
    semantic.continue_ptr.push(ptrAct);

    if (current_token.value != ";") {
        expression(lex);
        if (semantic.exprs.top().first.expr_type != ExprType::Bool || semantic.exprs.top().first.is_array)
            throw std::exception(("Invalid expression type. bool expected, but found " + semantic.type_to_string(semantic.exprs.top().first)).c_str());
        semantic.exprs.pop();

        prog.put_lex({ Poliz::PolizType::POINTER, ptrEnd });
        prog.put_lex({ Poliz::PolizType::FGO, new std::string("!F") });

        prog.put_lex({ Poliz::PolizType::POINTER, ptrBody });
        prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    }
    semicolon(lex, true);
    prog.blank();
    *ptrAct = prog.get_size() - 1;
    if (current_token.value != ")") {
        expression(lex);
        semantic.exprs.pop();
    }
    prog.put_lex({ Poliz::PolizType::POINTER, ptrExpr });
    prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    prog.blank();
    *ptrBody = prog.get_size() - 1;
    statement(lex, true);
    prog.put_lex({ Poliz::PolizType::POINTER, ptrAct });
    prog.put_lex({ Poliz::PolizType::GO, new std::string("!") });
    prog.blank();
    *ptrEnd = prog.get_size() - 1;
    semantic.break_ptr.pop();
    semantic.continue_ptr.pop();
}

void Analyzer::switch_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "switch")
        throw std::exception("Invalid token: 'switch' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    Type curr_type = semantic.exprs.top().first;
    semantic.exprs.pop();
    std::unordered_set<std::string> cases;
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    if (current_token.value != "{")
        throw std::exception("Invalid token: '{' expected");
    current_token = lex.get_token();
    int* ptr_end = new int(-1);
    prog.put_lex({ Poliz::PolizType::STACK_PLUG, nullptr });

    while (current_token.value == "case" || current_token.value == "default") {
        semantic.break_ptr.push(ptr_end);
        if (current_token.value == "default") {
            current_token = lex.get_token();
            if (cases.count("default") != 0)
                throw std::exception("The default case has already been described above");
            cases.insert("default");
            if (current_token.value != ":")
                throw std::exception("Invalid token: ':' expected");
            current_token = lex.get_token();

            statement(lex);
        } else {
            current_token = lex.get_token();
            if (current_token.type != LexicalAnalyzer::token_type::literal)
                throw std::exception("Invalid token: 'literal' expected");

            std::string val = current_token.value;
            if ((val[0] >= '0' && val[0] <= '9') || val[0] == '-' || val[0] == '+') {
                while (val.back() < '0' || val.back() > '9')
                    val.pop_back();
                val = std::to_string(std::stold(val));
            }
            if (cases.count(val) != 0)
                throw std::exception(("The case " + current_token.value + " has already been described above").c_str());
            if (!semantic.is_convertible(semantic.literal_to_type(lex, current_token), curr_type))
                throw std::exception(("Cannot cast " + semantic.type_to_string(semantic.literal_to_type(lex, current_token)) + " type to " + semantic.type_to_string(curr_type)).c_str());

            cases.insert(val);
            current_token = lex.get_token();
            if (current_token.value != ":")
                throw std::exception("Invalid token: ':' expected");
            current_token = lex.get_token();

            int* ptr_case_end = new int(-1);
            prog.put_lex({ Poliz::PolizType::LITERAL, new std::pair<std::string, ExprType>(val, curr_type.expr_type) });
            prog.put_lex({ Poliz::PolizType::SWITCH_CMP, new std::string("==") });
            prog.put_lex({ Poliz::PolizType::POINTER, ptr_case_end });
            prog.put_lex({ Poliz::PolizType::FGO, new std::string("!F") });

            statement(lex);
            prog.blank();
            *ptr_case_end = prog.get_size() - 1;
        }
        semantic.break_ptr.pop();
    }
    if (current_token.value != "}")
        throw std::exception("Invalid token: '}' expected");
    current_token = lex.get_token();

    prog.put_lex({ Poliz::PolizType::SWITCH_POP, new std::string("POP") });
    *ptr_end = prog.get_size() - 1;
}

void Analyzer::print_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "print")
        throw std::exception("Invalid token: 'print' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex, false, true);
    semantic.exprs.pop();
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::read_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "read")
        throw std::exception("Invalid token: 'read' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    field(lex, true);
    semantic.exprs.pop();
    prog.put_lex({ Poliz::PolizType::READ, nullptr });
    while (current_token.value == ",") {
        current_token = lex.get_token();
        field(lex, true);
        semantic.exprs.pop();
        prog.put_lex({ Poliz::PolizType::READ, nullptr });
    }
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::readln_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "readln")
        throw std::exception("Invalid token: 'readln' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    field(lex, true);
    auto type = semantic.exprs.top().first;
    if (type.expr_type != ExprType::String)
        throw std::exception("Cannot use readln with not a string identifier");
    semantic.exprs.pop();
    prog.put_lex({ Poliz::PolizType::READLN, nullptr });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::statement(LexicalAnalyzer& lex, bool prev_table) {
    if (current_token.value == "{") {
        current_token = lex.get_token();
        if (!prev_table)
            semantic.create_table();
        while (current_token.value != "}" && current_token.line != -1)
            statement(lex);
        if (current_token.value != "}")
            throw std::exception("Invalid token: '}' expected");
        current_token = lex.get_token();
        semantic.del_table();
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
        may_be_semicolon(lex);
        return;
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
    if (current_token.value == "read") {
        read_statement(lex);
        semicolon(lex);
        may_be_semicolon(lex);
        return;
    }
    if (current_token.value == "print") {
        print_statement(lex);
        semicolon(lex);
        may_be_semicolon(lex);
        return;
    }
    if (current_token.value == "readln") {
        readln_statement(lex);
        semicolon(lex);
        may_be_semicolon(lex);
        return;
    }
    if (current_token.value == "exit") {
        exit_statement(lex);
        semicolon(lex);
        may_be_semicolon(lex);
        return;
    }
    if (current_token.value == "convert") {
        convert_statement(lex);
        semicolon(lex);
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
            semantic.exprs.pop();
            semicolon(lex);
            may_be_semicolon(lex);
        } else {
            may_be_semicolon(lex);
        }
    }
}

void Analyzer::exit_statement(LexicalAnalyzer& lex) {
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    prog.put_lex({ Poliz::PolizType::EXIT, nullptr });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::strlen_statement(LexicalAnalyzer& lex) {
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    if (semantic.exprs.top().first.expr_type != ExprType::String)
        throw std::exception("Invalid operand: cannot use strlen function with not a string");
    semantic.exprs.pop();
    semantic.exprs.push({ Type(ExprType::Int, false, false), false });
    prog.put_lex({ Poliz::PolizType::STRLEN, nullptr });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::convert_statement(LexicalAnalyzer& lex) {
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex, true);
    if (current_token.value != ",")
        throw std::exception("Invalid token: ',' expected");
    current_token = lex.get_token();
    std::string* val = new std::string(current_token.value);
    default_type(lex);
    semantic.exprs.pop();
    semantic.exprs.push({ Type(semantic.string_to_type(*val), false, false), false });
    prog.put_lex({ Poliz::PolizType::CONVERT, val });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void Analyzer::rand_statement(LexicalAnalyzer& lex) {
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    prog.put_lex({ Poliz::PolizType::RAND, nullptr });
    semantic.exprs.push({ Type(ExprType::Int, false, false), false });
}

void Analyzer::time_statement(LexicalAnalyzer& lex) {
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    prog.put_lex({ Poliz::PolizType::TIME, nullptr });
    semantic.exprs.push({ Type(ExprType::Long, false, false), false });
}