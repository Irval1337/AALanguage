#include "Semantic.h"

ExprType Semantic::string_to_type(std::string str) {
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
    if (str == "large") return ExprType::Large;
    return ExprType::Unknown;
}

bool Semantic::is_convertible(Type first, Type second, bool is_func) {
    if (first.is_array != second.is_array || !is_func && first.array_size != second.array_size) return false;
    if (first == second) return true;
    switch (first.expr_type) {
    case Bool:
        return second.expr_type == ExprType::Bool;
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
    case Large:
        return second.expr_type != ExprType::Bool && second.expr_type != ExprType::String && second.expr_type != ExprType::Void && second.expr_type != ExprType::Unknown;
    case String:
        return second.expr_type == ExprType::String;
    case Void:
    case Unknown:
        return second.expr_type == ExprType::Unknown;
    default:
        break;
    }
}

Type Semantic::calc_expr_type(Type first, Type second) {
    if (!is_convertible(first, second)) return Type();
    if (type_prior[first.expr_type] >= type_prior[second.expr_type])
        return first;
    else
        return second;
}

std::string Semantic::type_to_string(Type type) {
    switch (type.expr_type) {
    case Bool:
        return (std::string)"bool" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Char:
        return (std::string)"char" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Double:
        return (std::string)"double" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case UDouble:
        return (std::string)"udouble" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Float:
        return (std::string)"float" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case UFloat:
        return (std::string)"ufloat" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Int:
        return (std::string)"int" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case UInt:
        return (std::string)"uint" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Long:
        return (std::string)"long" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case ULong:
        return (std::string)"ulong" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Short:
        return (std::string)"short" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case UShort:
        return (std::string)"ushort" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Byte:
        return (std::string)"byte" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case String:
        return (std::string)"string" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Large:
        return (std::string)"large" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Void:
        return (std::string)"void" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Unknown:
    default:
        return (std::string)"unknown" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    }
}

void Semantic::add_identifier(Identifier* ident, bool is_global) {
    if (!is_global) {
        if (tid->identifiers.count(ident->name) != 0) throw std::exception(("Identifier '" + ident->name + "' was already declared in the current scope").c_str());
        tid->identifiers[ident->name] = ident;
        params_ptrs.push_back(ident);
    } else {
        if (global_tid->identifiers.count(ident->name) != 0) throw std::exception(("Identifier '" + ident->name + "' was already declared in the current scope").c_str());
        global_tid->identifiers[ident->name] = ident;
    }
}

Identifier* Semantic::get_identifier(std::string name) {
    TableIdentifiers* ptr = tid;
    while (ptr != nullptr) {
        if (ptr->identifiers.count(name) != 0) {
            return ptr->identifiers[name];
        }
        ptr = ptr->parent;
    }
    if (global_tid->identifiers.count(name) == 0) {
        throw std::exception(("Identifier '" + name + "' was not declared in the current scope").c_str());
        return nullptr;
    }
    return global_tid->identifiers[name];
}

void Semantic::del_table() {
    auto next = tid->parent;
    delete tid;
    tid = next;
    if (tid == nullptr)
        tid = new TableIdentifiers();
}

void Semantic::create_table(bool new_func) {
    auto next = new TableIdentifiers();
    if (!new_func)
        next->parent = tid;
    tid = next;
}

void Semantic::create_function(std::string name, Type type, int pref, std::vector<int> inits, std::vector<Identifier*> ptrs, int ptr) {
    std::vector<Type> idents;
    for (auto& u : tid->identifiers) {
        idents.push_back(u.second->type);
    }
    Function f = Function(name, type, tid, idents, inits, ptrs, ptr, pref);
    if (funcs.count(f) != 0)
        throw std::exception(("Function '" + f.name + "' was already declared in the current scope").c_str());
    funcs[f] = tid;
    current_function = type;
}

Function Semantic::get_function(std::string name, std::vector<Type> params) {
    std::vector<std::pair<int, Function>> functions;
    for (auto& u : funcs) {
        int curr_level = 0;
        if (u.first.name != name) continue;
        if (params.size() < u.first.not_default_pref) continue;
        if (params.size() > u.first.identifiers.size()) continue;
        bool flag = true;
        for (int i = 0; i < params.size(); ++i) {
            if (params[i].expr_type != u.first.identifiers[i].expr_type) {
                if (!is_convertible(params[i], u.first.identifiers[i], true)) {
                    flag = false;
                    break;
                } else {
                    curr_level = 1;
                }
            }
            if (params[i].is_array != u.first.identifiers[i].is_array) {
                flag = false;
                break;
            }
        }

        if (flag) {
            functions.push_back({ curr_level, u.first });
        }
    }
    if (functions.empty()) {
        throw std::exception(("Function '" + name + "' was not declared in the current scope").c_str());
    }
    std::sort(functions.begin(), functions.end(), [&](std::pair<int, Function> f, std::pair<int, Function> s) {
        return f.first < s.first;
        });
    if (functions.size() == 1 || functions[1].first > functions[0].first)
        return functions[0].second;
    throw std::exception((name + " function call is ambiguous").c_str());
}

void Semantic::init_array(ExprType type, int size, void*& value, std::string name) {
    if (size > 5e5) throw std::exception("Array size cannot be bigger than 5e5");
    if (size <= 0) throw std::exception("Array size cannot be less than 1");
    std::vector<Identifier*>* arr = new std::vector<Identifier*>();
    for (int i = 0; i < size; ++i) {
        arr->push_back(new Identifier(name + "[" + std::to_string(i) + "]", Type(type, false, false), nullptr));
    }
    value = arr;
}

bool Semantic::check_operation(Type f, Type s, std::string operation) {
    auto first = f.expr_type, second = s.expr_type;
    if (operation == ">>" || operation == "<<" || operation == "~" || operation == "|" || operation == "&" || operation == "^" || operation == "->")
        if (first == ExprType::Double || first == ExprType::Float || second == ExprType::Double || second == ExprType::Float || second == ExprType::Large)
            return false;
    if (operation == "+")
        return first != ExprType::Bool && is_convertible(f, s);
    if (operation == "-" || operation == "*" || operation == "/" || operation == ">>" || operation == "<<" || operation == "<" || operation == "<=" ||
        operation == ">" || operation == ">=")
        return first != ExprType::Bool && first != ExprType::String && is_convertible(f, s);
    if (operation == "%")
        return first != ExprType::Bool && first != ExprType::String && first != ExprType::Float && first != ExprType::UFloat &&
        first != ExprType::Double && first != ExprType::UDouble && second != ExprType::Float && second != ExprType::UFloat &&
        second != ExprType::Double && second != ExprType::UDouble && is_convertible(f, s);
    if (operation == "&" || operation == "^" || operation == "|" || operation == "->" || operation == "~")
        return first != ExprType::String && is_convertible(f, s);
    if (operation == "=" || operation == "==" || operation == "!=")
        return is_convertible(f, s);
    if (operation == "||" || operation == "&&")
        return first == ExprType::Bool && is_convertible(f, s);
    return false;
}

Type Semantic::literal_to_type(LexicalAnalyzer& lex, Token current_token) {
    switch (lex.is_literal(current_token.value)) {
    case LexicalAnalyzer::literal_type::logical:
        return Type(ExprType::Bool, false, false);
    case LexicalAnalyzer::literal_type::integer:
        if (current_token.value.find("ul") != current_token.value.npos || current_token.value.find("UL") != current_token.value.npos)
            return Type(ExprType::ULong, false, false);
        if (current_token.value.find("l") != current_token.value.npos || current_token.value.find("L") != current_token.value.npos)
            return Type(ExprType::Long, false, false);
        if (current_token.value.find("i") != current_token.value.npos || current_token.value.find("I") != current_token.value.npos)
            return Type(ExprType::Int, false, false);
        if (current_token.value.find("u") != current_token.value.npos || current_token.value.find("U") != current_token.value.npos ||
            current_token.value.find("ui") != current_token.value.npos || current_token.value.find("UI") != current_token.value.npos)
            return Type(ExprType::UInt, false, false);
        if (current_token.value.find("s") != current_token.value.npos || current_token.value.find("S") != current_token.value.npos)
            return Type(ExprType::Short, false, false);
        if (current_token.value.find("us") != current_token.value.npos || current_token.value.find("US") != current_token.value.npos)
            return Type(ExprType::UShort, false, false);
        if (current_token.value.find("b") != current_token.value.npos || current_token.value.find("B") != current_token.value.npos)
            return Type(ExprType::Byte, false, false);
        return Type(ExprType::Int, false, false);
    case LexicalAnalyzer::literal_type::real:
        if (current_token.value.find("d") != current_token.value.npos || current_token.value.find("D") != current_token.value.npos)
            return Type(ExprType::Double, false, false);
        if (current_token.value.find("ud") != current_token.value.npos || current_token.value.find("UD") != current_token.value.npos)
            return Type(ExprType::UDouble, false, false);
        if (current_token.value.find("f") != current_token.value.npos || current_token.value.find("F") != current_token.value.npos)
            return Type(ExprType::Float, false, false);
        if (current_token.value.find("uf") != current_token.value.npos || current_token.value.find("UF") != current_token.value.npos)
            return Type(ExprType::UFloat, false, false);
        return Type(ExprType::Float, false, false);
    case LexicalAnalyzer::literal_type::symbol:
        return Type(ExprType::Char, false, false);
    case LexicalAnalyzer::literal_type::string:
        return Type(ExprType::String, false, false);
    default:
        return Type(ExprType::Unknown, false, false);
    }
}
