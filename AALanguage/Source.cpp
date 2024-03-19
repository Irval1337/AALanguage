#include <regex>
#include <iostream>
#include "LexicalAnalyzer.h"
#include <exception>
#include <stack>
#include "TableIdentifiers.h"
#include <unordered_set>
#include <string>

enum PolizType {
    GO, FGO, TGO, LABEL, ADDRESS, POINTER, GETARR, CALL, BLANK, SEMICOLON, LITERAL, USING, COMMA, ASSIGN, LOGICAL_OR, 
    LOGICAL_AND, BITWISE_OR, BITWISE_XOR, BITWISE_CONS, BITWISE_AND, EQUALITY, CMP, BITWISE_SHIFT, PLUS, MULT, UNARY, 
    SWITCH_CMP, SWITCH_POP, RET, BOOL_LITERAL, BYTE_LITERAL, CHAR_LITERAL, DOUBLE_LITERAL, FLOAT_LITERAL, INT_LITERAL, 
    LONG_LITERAL, SHORT_LITERAL, STRING_LITERAL, UINT_LITERAL, ULONG_LITERAL, STACK_PLUG, READ, PRINT, READLN
};

TableIdentifiers* tid = new TableIdentifiers();

class Poliz {
private:
    int literal_prior[11] = { 0, 2, 1, 11, 7, 5, 9, 3, 0, 6, 10 };
    std::stack<int> func_stack;
public:
    ~Poliz() {
        for (int i = 0; i < lexes.size(); ++i) {
            if (lexes[i].first != PolizType::ADDRESS && lexes[i].first != PolizType::POINTER && lexes[i].second != nullptr)
                delete lexes[i].second;
        }
    }
    void put_lex(std::pair<PolizType, void*> l) {
        lexes.push_back(l);
    }
    void blank() {
        lexes.push_back({ PolizType::BLANK, nullptr });
    }
    int get_size() {
        return lexes.size();
    }
    void print() {
        for (int i = 0; i < lexes.size(); ++i) {
            if (lexes[i].first == PolizType::ADDRESS) {
                std::cout << ((Identifier*)lexes[i].second)->name << " ";
                continue;
            }
            if (lexes[i].first == PolizType::POINTER) {
                std::cout << *((int*)lexes[i].second) << " ";
                continue;
            }
            //std::cout << lexes[i].first;
            if (lexes[i].second != nullptr)
                std::cout << *((std::string*)lexes[i].second) << " ";
            //std::cout << std::endl;
        }
    }
    void* convert(std::pair<PolizType, void*> op, PolizType to) {
        if (op.first == PolizType::ADDRESS)
            op = address_to_value(op.second);

        switch (op.first) {
        case BOOL_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((bool*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((bool*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((bool*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((bool*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((bool*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((bool*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((bool*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((bool*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(*((bool*)op.second) ? "true" : "false");
                break;
            case UINT_LITERAL:
                return new uint32_t(*((bool*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((bool*)op.second));
                break;
            }
            break;
        }
        case BYTE_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((uint8_t*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((uint8_t*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((uint8_t*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((uint8_t*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((uint8_t*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((uint8_t*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((uint8_t*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((uint8_t*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((uint8_t*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((uint8_t*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((uint8_t*)op.second));
                break;
            }
            break;
        }
        case CHAR_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((char*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((char*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((char*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((char*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((char*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((char*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((char*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((char*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(1, *((char*)op.second));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((char*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((char*)op.second));
                break;
            }
            break;
        }
        case DOUBLE_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((double*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((double*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((double*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((double*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((double*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((double*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((double*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((double*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((double*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((double*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((double*)op.second));
                break;
            }
            break;
        }
        case FLOAT_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((float*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((float*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((float*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((float*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((float*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((float*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((float*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((float*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((float*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((float*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((float*)op.second));
                break;
            }
            break;
        }
        case INT_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((int*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((int*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((int*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((int*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((int*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((int*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((int*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((int*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((int*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((int*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((int*)op.second));
                break;
            }
            break;
        }
        case LONG_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((long long*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((long long*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((long long*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((long long*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((long long*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((long long*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((long long*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((long long*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((long long*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((long long*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((long long*)op.second));
                break;
            }
            break;
        }
        case SHORT_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((short*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((short*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((short*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((short*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((short*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((short*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((short*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((short*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((short*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((short*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((short*)op.second));
                break;
            }
            break;
        }
        case STRING_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((std::string*)op.second) == "true");
                break;
            case BYTE_LITERAL:
                return new uint8_t(std::stoll(*((std::string*)op.second)));
                break;
            case CHAR_LITERAL:
                return new char((*((std::string*)op.second))[0]);
                break;
            case DOUBLE_LITERAL:
                return new double(std::stold(*((std::string*)op.second)));
                break;
            case FLOAT_LITERAL:
                return new float(std::stold(*((std::string*)op.second)));
                break;
            case INT_LITERAL:
                return new int(std::stoll(*((std::string*)op.second)));
                break;
            case LONG_LITERAL:
                return new long long(std::stoll(*((std::string*)op.second)));
                break;
            case SHORT_LITERAL:
                return new short(std::stoll(*((std::string*)op.second)));
                break;
            case STRING_LITERAL:
                return new std::string(*((std::string*)op.second));
                break;
            case UINT_LITERAL:
                return new uint32_t(std::stoll(*((std::string*)op.second)));
                break;
            case ULONG_LITERAL:
                return new uint64_t(std::stoll(*((std::string*)op.second)));
                break;
            }
            break;
        } case UINT_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((uint32_t*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((uint32_t*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((uint32_t*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((uint32_t*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((uint32_t*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((uint32_t*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((uint32_t*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((uint32_t*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((uint32_t*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((uint32_t*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((uint32_t*)op.second));
                break;
            }
            break;
        } case ULONG_LITERAL: {
            switch (to) {
            case BOOL_LITERAL:
                return new bool(*((uint64_t*)op.second));
                break;
            case BYTE_LITERAL:
                return new uint8_t(*((uint64_t*)op.second));
                break;
            case CHAR_LITERAL:
                return new char(*((uint64_t*)op.second));
                break;
            case DOUBLE_LITERAL:
                return new double(*((uint64_t*)op.second));
                break;
            case FLOAT_LITERAL:
                return new float(*((uint64_t*)op.second));
                break;
            case INT_LITERAL:
                return new int(*((uint64_t*)op.second));
                break;
            case LONG_LITERAL:
                return new long long(*((uint64_t*)op.second));
                break;
            case SHORT_LITERAL:
                return new short(*((uint64_t*)op.second));
                break;
            case STRING_LITERAL:
                return new std::string(std::to_string(*((uint64_t*)op.second)));
                break;
            case UINT_LITERAL:
                return new uint32_t(*((uint64_t*)op.second));
                break;
            case ULONG_LITERAL:
                return new uint64_t(*((uint64_t*)op.second));
                break;
            }
            break;
        }
        }
    }
    std::pair<PolizType, std::pair<void*, void*>> make_same(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2) {
        int pr1 = literal_prior[(int)op1.first - (int)PolizType::BOOL_LITERAL], pr2 = literal_prior[(int)op2.first - (int)PolizType::BOOL_LITERAL];
        if (pr1 >= pr2)
            return std::pair<PolizType, std::pair<void*, void*>>(op1.first, { op1.second, convert(op2, op1.first) });
        else
            return std::pair<PolizType, std::pair<void*, void*>>(op2.first, { convert(op1, op2.first), op2.second });

    }
    std::pair<PolizType, void*> address_to_value(void* ptr) {
        Identifier* ident = (Identifier*)ptr;
        if (ident->value == nullptr) {
            throw std::exception(("Variable '" + ident->name + "' was not initialized").c_str());
        }
        switch (ident->type.expr_type) {
        case ExprType::Bool:
            return { PolizType::BOOL_LITERAL, (bool*)(ident->value) };
        case ExprType::Byte:
            return { PolizType::BYTE_LITERAL, (uint8_t*)(ident->value) };
        case ExprType::Char:
            return { PolizType::CHAR_LITERAL, (char*)(ident->value) };
        case ExprType::Double:
            return { PolizType::DOUBLE_LITERAL, (double*)(ident->value) };
        case ExprType::Float:
            return { PolizType::FLOAT_LITERAL, (float*)(ident->value) };
        case ExprType::Int:
            return { PolizType::INT_LITERAL, (int*)(ident->value) };
        case ExprType::Long:
            return { PolizType::LONG_LITERAL, (long long*)(ident->value) };
        case ExprType::Short:
            return { PolizType::SHORT_LITERAL, (short*)(ident->value) };
        case ExprType::String:
            return { PolizType::STRING_LITERAL, (std::string*)(ident->value) };
        case ExprType::UInt:
            return { PolizType::UINT_LITERAL, (uint32_t*)(ident->value) };
        case ExprType::ULong:
            return { PolizType::ULONG_LITERAL, (uint64_t*)(ident->value) };
        default:
            throw std::exception(("Variable '" + ident->name + "' has unconvertable value").c_str());
        }
    }
    PolizType type_to_poliz(ExprType type) {
        switch (type)
        {
        case Bool:
            return PolizType::BOOL_LITERAL;
        case Char:
            return PolizType::CHAR_LITERAL;
        case Double:
            return PolizType::DOUBLE_LITERAL;
        case Float:
            return PolizType::FLOAT_LITERAL;
        case Int:
            return PolizType::INT_LITERAL;
        case UInt:
            return PolizType::UINT_LITERAL;
        case Long:
            return PolizType::LONG_LITERAL;
        case ULong:
            return PolizType::ULONG_LITERAL;
        case Short:
            return PolizType::SHORT_LITERAL;
        case Byte:
            return PolizType::BYTE_LITERAL;
        case String:
            return PolizType::STRING_LITERAL;
        default:
            return PolizType::BLANK;
        }
    }
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op1, std::pair<PolizType, void*> op2, std::string operation) {
        if (op1.first == PolizType::ADDRESS)
            op1 = address_to_value(op1.second);
        if (op2.first == PolizType::ADDRESS)
            op2 = address_to_value(op2.second);
        auto conv = make_same(op1, op2);
        if (operation == "+") {
            switch (conv.first) {
                case BOOL_LITERAL:
                    return { conv.first, new bool(*((bool*)conv.second.first) + *((bool*)conv.second.second)) };
                case BYTE_LITERAL:
                    return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) + *((uint8_t*)conv.second.second)) };
                case CHAR_LITERAL:
                    return { conv.first, new char(*((char*)conv.second.first) + *((char*)conv.second.second)) };
                case DOUBLE_LITERAL:
                    return { conv.first, new double(*((double*)conv.second.first) + *((double*)conv.second.second)) };
                case FLOAT_LITERAL:
                    return { conv.first, new float(*((float*)conv.second.first) + *((float*)conv.second.second)) };
                case INT_LITERAL:
                    return { conv.first, new int(*((int*)conv.second.first) + *((int*)conv.second.second)) };
                case LONG_LITERAL:
                    return { conv.first, new long long(*((long long*)conv.second.first) + *((long long*)conv.second.second)) };
                case SHORT_LITERAL:
                    return { conv.first, new short(*((short*)conv.second.first) + *((short*)conv.second.second)) };
                case STRING_LITERAL:
                    return { conv.first, new std::string(*((std::string*)conv.second.first) + *((std::string*)conv.second.second)) };
                case UINT_LITERAL:
                    return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) + *((uint32_t*)conv.second.second)) };
                case ULONG_LITERAL:
                    return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) + *((uint64_t*)conv.second.second)) };
                default:
                    throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "-") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) - *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) - *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) - *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { conv.first, new double(*((double*)conv.second.first) - *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { conv.first, new float(*((float*)conv.second.first) - *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) - *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) - *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) - *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) - *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) - *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "*") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) * *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) * *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) * *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { conv.first, new double(*((double*)conv.second.first) * *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { conv.first, new float(*((float*)conv.second.first) * *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) * *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) * *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) * *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) * *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) * *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "/") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) / *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) / *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) / *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { conv.first, new double(*((double*)conv.second.first) / *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { conv.first, new float(*((float*)conv.second.first) / *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) / *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) / *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) / *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) / *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) / *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "%") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) % *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) % *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) % *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) % *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) % *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) % *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) % *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) % *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == ">>") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) >> *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) >> *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) >> *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) >> *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) >> *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) >> *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) >> *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) >> *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "<<") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) << *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) << *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) << *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) << *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) << *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) << *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) << *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) << *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "<") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) < *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) < *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) < *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) < *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) < *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) < *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) < *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) < *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) < *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) < *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) < *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "<=") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) <= *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) <= *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) <= *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) <= *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) <= *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) <= *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) <= *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) <= *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) <= *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) <= *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) <= *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == ">") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) > *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) > *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) > *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) > *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) > *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) > *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) > *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) > *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) > *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) > *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) > *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == ">=") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) >= *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) >= *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) >= *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) >= *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) >= *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) >= *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) >= *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) >= *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) >= *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) >= *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) >= *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "==") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) == *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) == *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) == *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) == *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) == *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) == *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) == *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) == *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) == *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) == *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) == *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "!=") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) != *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) != *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) != *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) != *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) != *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) != *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) != *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) != *((short*)conv.second.second)) };
            case STRING_LITERAL:
                return { BOOL_LITERAL, new bool(*((std::string*)conv.second.first) != *((std::string*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) != *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) != *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "&") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) & *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) & *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) & *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) & *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) & *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) & *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) & *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) & *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "|") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) | *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) | *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) | *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) | *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) | *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) | *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) | *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) | *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "^") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(*((bool*)conv.second.first) ^ *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(*((uint8_t*)conv.second.first) ^ *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(*((char*)conv.second.first) ^ *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(*((int*)conv.second.first) ^ *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(*((long long*)conv.second.first) ^ *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(*((short*)conv.second.first) ^ *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(*((uint32_t*)conv.second.first) ^ *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(*((uint64_t*)conv.second.first) ^ *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "->") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { conv.first, new bool(~*((bool*)conv.second.first) | *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { conv.first, new uint8_t(~*((uint8_t*)conv.second.first) | *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { conv.first, new char(~*((char*)conv.second.first) | *((char*)conv.second.second)) };
            case INT_LITERAL:
                return { conv.first, new int(~*((int*)conv.second.first) | *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { conv.first, new long long(~*((long long*)conv.second.first) | *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { conv.first, new short(~*((short*)conv.second.first) | *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { conv.first, new uint32_t(~*((uint32_t*)conv.second.first) | *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { conv.first, new uint64_t(~*((uint64_t*)conv.second.first) | *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "&&") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) && *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) && *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) && *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) && *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) && *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) && *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) && *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) && *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) && *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) && *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
        else if (operation == "&&") {
            switch (conv.first) {
            case BOOL_LITERAL:
                return { BOOL_LITERAL, new bool(*((bool*)conv.second.first) || *((bool*)conv.second.second)) };
            case BYTE_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint8_t*)conv.second.first) || *((uint8_t*)conv.second.second)) };
            case CHAR_LITERAL:
                return { BOOL_LITERAL, new bool(*((char*)conv.second.first) || *((char*)conv.second.second)) };
            case DOUBLE_LITERAL:
                return { BOOL_LITERAL, new bool(*((double*)conv.second.first) || *((double*)conv.second.second)) };
            case FLOAT_LITERAL:
                return { BOOL_LITERAL, new bool(*((float*)conv.second.first) || *((float*)conv.second.second)) };
            case INT_LITERAL:
                return { BOOL_LITERAL, new bool(*((int*)conv.second.first) || *((int*)conv.second.second)) };
            case LONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((long long*)conv.second.first) || *((long long*)conv.second.second)) };
            case SHORT_LITERAL:
                return { BOOL_LITERAL, new bool(*((short*)conv.second.first) || *((short*)conv.second.second)) };
            case UINT_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint32_t*)conv.second.first) || *((uint32_t*)conv.second.second)) };
            case ULONG_LITERAL:
                return { BOOL_LITERAL, new bool(*((uint64_t*)conv.second.first) || *((uint64_t*)conv.second.second)) };
            default:
                throw std::exception("Cannot use this operation");
            }
        }
    }
    std::pair<PolizType, void*> execute_operation(std::pair<PolizType, void*> op, std::string operation) {
        Identifier* ident = nullptr;
        if (op.first == PolizType::ADDRESS) {
            ident = (Identifier*)op.second;
            op = address_to_value(op.second);
        }
        if (operation == "++") {
            switch (op.first) {
            case BOOL_LITERAL: {
                auto val = *((bool*)op.second);
                ++val;
                ident->value = new bool(val);
                return { ADDRESS, ident };
            }
            case BYTE_LITERAL: {
                auto val = *((uint8_t*)op.second);
                ++val;
                ident->value = new uint8_t(val);
                return { ADDRESS, ident };
            }
            case CHAR_LITERAL: {
                auto val = *((char*)op.second);
                ++val;
                ident->value = new char(val);
                return { ADDRESS, ident };
            }
            case DOUBLE_LITERAL: {
                auto val = *((double*)op.second);
                ++val;
                ident->value = new double(val);
                return { ADDRESS, ident };
            }
            case FLOAT_LITERAL: {
                auto val = *((float*)op.second);
                ++val;
                ident->value = new float(val);
                return { ADDRESS, ident };
            }
            case INT_LITERAL: {
                auto val = *((int*)op.second);
                ++val;
                ident->value = new int(val);
                return { ADDRESS, ident };
            }
            case LONG_LITERAL: {
                auto val = *((long long*)op.second);
                ++val;
                ident->value = new long long(val);
                return { ADDRESS, ident };
            }
            case SHORT_LITERAL: {
                auto val = *((short*)op.second);
                ++val;
                ident->value = new short(val);
                return { ADDRESS, ident };
            }
            case UINT_LITERAL: {
                auto val = *((uint32_t*)op.second);
                ++val;
                ident->value = new uint32_t(val);
                return { ADDRESS, ident };
            }
            case ULONG_LITERAL: {
                auto val = *((uint64_t*)op.second);
                ++val;
                ident->value = new uint64_t(val);
                return { ADDRESS, ident };
            }
            }
        }
        else if (operation == "--") {
            switch (op.first) {
            case BYTE_LITERAL: {
                auto val = *((uint8_t*)op.second);
                --val;
                ident->value = new uint8_t(val);
                return { ADDRESS, ident };
            }
            case CHAR_LITERAL: {
                auto val = *((char*)op.second);
                --val;
                ident->value = new char(val);
                return { ADDRESS, ident };
            }
            case DOUBLE_LITERAL: {
                auto val = *((double*)op.second);
                --val;
                ident->value = new double(val);
                return { ADDRESS, ident };
            }
            case FLOAT_LITERAL: {
                auto val = *((float*)op.second);
                --val;
                ident->value = new float(val);
                return { ADDRESS, ident };
            }
            case INT_LITERAL: {
                auto val = *((int*)op.second);
                --val;
                ident->value = new int(val);
                return { ADDRESS, ident };
            }
            case LONG_LITERAL: {
                auto val = *((long long*)op.second);
                --val;
                ident->value = new long long(val);
                return { ADDRESS, ident };
            }
            case SHORT_LITERAL: {
                auto val = *((short*)op.second);
                --val;
                ident->value = new short(val);
                return { ADDRESS, ident };
            }
            case UINT_LITERAL: {
                auto val = *((uint32_t*)op.second);
                --val;
                ident->value = new uint32_t(val);
                return { ADDRESS, ident };
            }
            case ULONG_LITERAL: {
                auto val = *((uint64_t*)op.second);
                --val;
                ident->value = new uint64_t(val);
                return { ADDRESS, ident };
            }
            }
        }
        else if (operation == "+") {
            return op;
        }
        else if (operation == "-") {
            switch (op.first) {
            case BOOL_LITERAL:
                return { op.first, new bool(-*((bool*)op.second)) };
            case BYTE_LITERAL:
                return { op.first, new uint8_t(-*((uint8_t*)op.second)) };
            case CHAR_LITERAL:
                return { op.first, new char(-*((char*)op.second)) };
            case DOUBLE_LITERAL:
                return { op.first, new double(-*((double*)op.second)) };
            case FLOAT_LITERAL:
                return { op.first, new float(-*((float*)op.second)) };
            case INT_LITERAL:
                return { op.first, new int(-*((int*)op.second)) };
            case LONG_LITERAL:
                return { op.first, new long long(-*((long long*)op.second)) };
            case SHORT_LITERAL:
                return { op.first, new short(-*((short*)op.second)) };
            }
        }
        else if (operation == "!") {
            switch (op.first) {
            case BOOL_LITERAL:
                return { op.first, new bool(!*((bool*)op.second)) };
            case BYTE_LITERAL:
                return { op.first, new uint8_t(!*((uint8_t*)op.second)) };
            case CHAR_LITERAL:
                return { op.first, new char(!*((char*)op.second)) };
            case DOUBLE_LITERAL:
                return { op.first, new double(!*((double*)op.second)) };
            case FLOAT_LITERAL:
                return { op.first, new float(!*((float*)op.second)) };
            case INT_LITERAL:
                return { op.first, new int(!*((int*)op.second)) };
            case LONG_LITERAL:
                return { op.first, new long long(!*((long long*)op.second)) };
            case SHORT_LITERAL:
                return { op.first, new short(!*((short*)op.second)) };
            case UINT_LITERAL:
                return { op.first, new uint32_t(!*((uint32_t*)op.second)) };
            case ULONG_LITERAL:
                return { op.first, new uint64_t(!*((uint64_t*)op.second)) };
            }
        }
        else if (operation == "~") {
            switch (op.first) {
            case BOOL_LITERAL:
                return { op.first, new bool(~*((bool*)op.second)) };
            case BYTE_LITERAL:
                return { op.first, new uint8_t(~*((uint8_t*)op.second)) };
            case CHAR_LITERAL:
                return { op.first, new char(~*((char*)op.second)) };
            case INT_LITERAL:
                return { op.first, new int(~*((int*)op.second)) };
            case LONG_LITERAL:
                return { op.first, new long long(~*((long long*)op.second)) };
            case SHORT_LITERAL:
                return { op.first, new short(~*((short*)op.second)) };
            case UINT_LITERAL:
                return { op.first, new uint32_t(~*((uint32_t*)op.second)) };
            case ULONG_LITERAL:
                return { op.first, new uint64_t(~*((uint64_t*)op.second)) };
            }
        }
    }
    void call_function(std::stack<std::pair<PolizType, void*>>& st, int& p) {
        int sz = *(int*)st.top().second;
        st.pop();
        Function* func = (Function*)st.top().second;
        st.pop();
        std::stack<std::pair<PolizType, void*>> args;
        for (int i = 0; i < sz; ++i) {
            args.push(st.top());
            st.pop();
        }
        int q = 0;
        while (!st.empty()) {
            a = convert(st.top(), type_to_poliz(var->type.expr_type));
            st.pop();
            p = func->params_init[q];
            ++q;
        }
    }
    void execute(int entrypoint) {
        std::stack<std::pair<PolizType, void*>> st;
        int p = entrypoint;
        while (p != lexes.size()) {
            switch (lexes[p].first) {
            case GO: {
                int ind = *((int*)st.top().second);
                st.pop();
                p = ind;
                break;
            }
            case FGO: {
                int ind = *((int*)st.top().second);
                st.pop();
                bool val = *((bool*)st.top().second) == false;
                st.pop(); 
                if (val) {
                    p = ind;
                } else ++p;
                break;
            }
            case TGO: {
                int ind = *((int*)st.top().second);
                st.pop();
                bool val = *((bool*)st.top().second) == true;
                st.pop();
                if (val) {
                    p = ind;
                } else ++p;
                break;
            }
            case BLANK:
            case LABEL: {
                ++p;
                break;
            }
            case BOOL_LITERAL:
            case BYTE_LITERAL:
            case CHAR_LITERAL:
            case DOUBLE_LITERAL:
            case FLOAT_LITERAL:
            case INT_LITERAL:
            case LONG_LITERAL:
            case SHORT_LITERAL:
            case STRING_LITERAL:
            case UINT_LITERAL:
            case ULONG_LITERAL:
            case ADDRESS:
            case STACK_PLUG:
            case POINTER: {
                st.push(lexes[p]);
                ++p;
                break;
            }
            case GETARR: {
                int ind = *(int*)convert(st.top(), PolizType::INT_LITERAL);
                st.pop();
                if (st.top().second == nullptr)
                    throw std::exception("Cannot access the character of an uninitialized string");
                auto var = (std::vector<void*>*)((Identifier*)st.top().second)->value;
                st.pop();
                if (var->size() <= ind)
                    throw std::exception("Array index out of range");
                st.push({ PolizType::ADDRESS, var->operator[](ind) });
                ++p;
                break;
            }
            case SEMICOLON: {
                while (!st.empty() && st.top().first != PolizType::STACK_PLUG) {
                    st.pop();
                }
                ++p;
                break;
            }
            case LITERAL: {
                std::pair<std::string, Type> val = *((std::pair<std::string, Type>*)lexes[p].second);
                if (val.second.expr_type == ExprType::Bool) {
                    st.push({ PolizType::BOOL_LITERAL, new bool(val.first == "true") });
                } else if (val.second.expr_type == ExprType::Byte || val.second.expr_type == ExprType::UShort) {
                    st.push({ PolizType::BYTE_LITERAL, new uint8_t(std::stoll(val.first)) });
                } else if (val.second.expr_type == ExprType::Char) {
                    std::string s = val.first.substr(1, val.first.size() - 2);
                    if (s == "\\\'")
                        st.push({ PolizType::CHAR_LITERAL, new char('\'') });
                    else if (s == "\\\"")
                        st.push({ PolizType::CHAR_LITERAL, new char('\"') });
                    else if (s == "\\n")
                        st.push({ PolizType::CHAR_LITERAL, new char('\n') });
                    else if (s == "\\t")
                        st.push({ PolizType::CHAR_LITERAL, new char('\t') });
                    else if (s == "\\0")
                        st.push({ PolizType::CHAR_LITERAL, new char('\0') });
                    else
                        st.push({ PolizType::CHAR_LITERAL, new char(s[0]) });
                } else if (val.second.expr_type == ExprType::Double) {
                    st.push({ PolizType::DOUBLE_LITERAL, new double(std::stold(val.first)) });
                } else if (val.second.expr_type == ExprType::Float) {
                    st.push({ PolizType::FLOAT_LITERAL, new float(std::stold(val.first)) });
                } else if (val.second.expr_type == ExprType::Int) {
                    st.push({ PolizType::INT_LITERAL, new int(std::stoll(val.first)) });
                } else if (val.second.expr_type == ExprType::Long) {
                    st.push({ PolizType::LONG_LITERAL, new long long(std::stoll(val.first)) });
                } else if (val.second.expr_type == ExprType::Short) {
                    st.push({ PolizType::SHORT_LITERAL, new short(std::stoll(val.first)) });
                } else if (val.second.expr_type == ExprType::String) {
                    st.push({ PolizType::STRING_LITERAL, new std::string(val.first.substr(1, val.first.size() - 2)) });
                } else if (val.second.expr_type == ExprType::UInt) {
                    st.push({ PolizType::UINT_LITERAL, new uint32_t(std::stoll(val.first)) });
                } else if (val.second.expr_type == ExprType::ULong) {
                    st.push({ PolizType::ULONG_LITERAL, new uint64_t(std::stoll(val.first)) });
                }
                ++p;
                break;
            }
            case COMMA: {
                auto op2 = st.top();
                st.pop();
                auto op1 = st.top();
                st.pop();
                st.push(op2);
                ++p;
                break;
            }
            case ASSIGN: {
                auto op2 = st.top();
                st.pop();
                auto op1 = st.top();
                st.pop();
                auto var = (Identifier*)op1.second;
                auto assign_val = op2.second;
                if (op2.first == PolizType::ADDRESS) {
                    assign_val = ((Identifier*)op2.second)->value;
                    if (assign_val == nullptr)
                        throw std::exception(("Cannot use a variable '" + ((Identifier*)op2.second)->name + "' that has not been assigned a value").c_str());
                }

                if (*((std::string*)lexes[p].second) == "=") {
                    var->value = convert(op2, type_to_poliz(var->type.expr_type));
                }
                else {
                    std::string op = *((std::string*)lexes[p].second);
                    auto val = execute_operation(op1, op2, op.substr(0, op.size() - 1));
                    var->value = convert(val, type_to_poliz(var->type.expr_type));
                }
                st.push(op1);
                ++p;
                break;
            }
            case UNARY: {
                auto op = st.top();
                st.pop();
                st.push(execute_operation(op, *((std::string*)lexes[p].second)));
                ++p;
                break;
            }
            case RET: {
                ++p;
                break;
            }
            case SWITCH_CMP: {
                auto op2 = st.top();
                st.pop();
                st.pop();
                auto op1 = st.top();
                st.pop();
                st.push(op1);
                st.push({ PolizType::STACK_PLUG, nullptr });
                st.push(execute_operation(op1, op2, "=="));
                ++p;
                break;
            }
            case SWITCH_POP: {
                while (st.top().first != PolizType::STACK_PLUG) {
                    st.pop();
                }
                st.pop();
                st.pop();
                ++p;
                break;
            }
            case READ: {
                auto var = (Identifier*)st.top().second;
                st.pop();
                std::string value;
                std::cin >> value;
                var->value = convert({ PolizType::STRING_LITERAL, new std::string(value) }, type_to_poliz(var->type.expr_type));
                ++p;
                break;
            }
            case READLN: {
                auto var = (Identifier*)st.top().second;
                st.pop();
                std::string value;
                std::cin.clear();
                std::getline(std::cin, value);
                var->value = convert({ PolizType::STRING_LITERAL, new std::string(value) }, type_to_poliz(var->type.expr_type));
                ++p;
                break;
            }
            case PRINT: {
                auto op = st.top();
                st.pop();
                std::cout << *(std::string*)(convert(op, PolizType::STRING_LITERAL));
                ++p;
                break;
            }
            default:
                auto op2 = st.top();
                st.pop();
                auto op1 = st.top();
                st.pop();
                st.push(execute_operation(op1, op2, *(std::string*)(lexes[p].second)));
                ++p;
                break;
            }
        }
    }
    std::pair<PolizType, void*>& operator[](int ind) {
        return lexes[ind];
    }
private:
    std::vector<std::pair<PolizType, void*>> lexes;
};

Token current_token;
std::stack<std::pair<Type, bool>> exprs;
std::unordered_map<Function, TableIdentifiers*, FunctionHasher> funcs;
TableIdentifiers* global_tid = new TableIdentifiers();
Type current_function;
std::stack<int*> break_ptr, continue_ptr;
Poliz prog;

bool is_convertible(Type first, Type second, bool is_func = false) {
    if (first.is_array != second.is_array || !is_func && first.array_size != second.array_size) return false;
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

Type calc_expr_type(Type first, Type second) {
    if (!is_convertible(first, second)) return Type();
    if (type_prior[first.expr_type] >= type_prior[second.expr_type])
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

std::string type_to_string(Type type) {
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
    case Void:
        return (std::string)"void" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    case Unknown:
    default:
        return (std::string)"unknown" + (type.is_array ? "[" + std::to_string(type.array_size) + "]" : "");
    }
}

void add_identifier(Identifier* ident, bool is_global = false) {
    if (!is_global) {
        if (tid->identifiers.count(ident->name) != 0) throw std::exception(("Identifier '" + ident->name + "' was already declared in the current scope").c_str());
        tid->identifiers[ident->name] = ident;
    } else {
        if (global_tid->identifiers.count(ident->name) != 0) throw std::exception(("Identifier '" + ident->name + "' was already declared in the current scope").c_str());
        global_tid->identifiers[ident->name] = ident;
    }
}

Identifier* get_identifier(std::string name) {
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

void create_function(std::string name, Type type, int pref, std::vector<int> inits, int ptr) {
    std::vector<Type> idents;
    Function f = Function(name, type, idents, inits, ptr, pref);
    if (funcs.count(f) != 0)
        throw std::exception(("Function '" + f.name + "' was already declared in the current scope").c_str());
    funcs[f] = tid;
    current_function = type;
}

Function get_function(std::string name, std::vector<Type> params) {
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

std::vector<std::string> service_types = { "bool", "char", "byte", "double", "udouble", "float", "ufloat", "int", "uint", "long",
        "ulong", "short", "ushort", "string" };

int function_params_pref = 0;
bool is_in_function_header = false, was_function_assign = false;

void semicolon(LexicalAnalyzer& lex, bool is_important) {
    if (current_token.line == -1)
        throw std::exception("Invalid end of line: ';' expected");
    if (current_token.value != ";")
        throw std::exception("Invalid token: ';' expected");
    prog.put_lex({ PolizType::SEMICOLON, new std::string(";") });
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
        prog.put_lex({ PolizType::SEMICOLON, new std::string(";") });
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
    prog.put_lex({ PolizType::USING, new std::string(curr_namespace) });
}

void program_body(LexicalAnalyzer& lex) {
    if (current_token.line == -1) return;
    function_or_var_definitions(lex, true);
    may_be_semicolon(lex);
    program_body(lex);
}

void init_array(ExprType type, int size, void*& value, std::string name) {
    std::vector<Identifier*>* arr = new std::vector<Identifier*>();
    for (int i = 0; i < size; ++i) {
        arr->push_back(new Identifier(name + "[" + std::to_string(i) + "]", Type(type, false, false), nullptr));
    }
    value = arr;
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
        prog.put_lex({ PolizType::ADDRESS, ident });
        expression(lex, true);
        if (!is_convertible(exprs.top().first, ident->type))
            throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(ident->type)).c_str());
        exprs.pop();
        prog.put_lex({ PolizType::ASSIGN, new std::string("=")});

        if (is_in_function_header) {
            if (!was_function_assign)
                was_function_assign = true;
        }
    } else if (is_in_function_header) {
        if (was_function_assign)
            throw std::exception("Parameters with default values cannot be followed by standard ones");
        ++function_params_pref;
    }
    if (current_token.value != "=") {
        if (is_array) {
            init_array(curr_type, size, ident->value, ident->name);
        } else ident->value = nullptr;
    }
    add_identifier(ident);
}

void var_definitions(LexicalAnalyzer& lex, bool need_semicolon, bool is_program) {
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
        prog.put_lex({ PolizType::ADDRESS, ident });
        expression(lex, true);
        prog.put_lex({ PolizType::ASSIGN, new std::string("=")});

        if (!is_convertible(exprs.top().first, ident->type))
            throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(ident->type)).c_str());
        exprs.pop();
    } else {
        if (is_array) init_array(curr_type, size, ident->value, ident->name);
        else ident->value = nullptr;
    }
    add_identifier(ident, is_program);
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
            prog.put_lex({ PolizType::ADDRESS, ident });
            expression(lex, true);
            prog.put_lex({ PolizType::ASSIGN, new std::string("=")});

            if (!is_convertible(exprs.top().first, ident->type))
                throw std::exception(("Cannot convert " + type_to_string(exprs.top().first) + " to " + type_to_string(ident->type)).c_str());
            exprs.pop();
        } else {
            if (is_array) init_array(curr_type, size, ident->value, ident->name);
            else ident->value = nullptr;
        }
        add_identifier(ident, is_program);
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

std::vector<int> function_params_init;

void function(LexicalAnalyzer& lex, bool is_struct) {
    ExprType curr_type = string_to_type(current_token.value);
    if (current_token.value == "void")
        current_token = lex.get_token();
    else
        default_type(lex);

    function_params_pref = 0;
    Type func_type;
    func_type.expr_type = curr_type;
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    std::unordered_map<std::string, void*> values;

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

    create_table(true);
    bool was_parameters = false;
    int ptr = prog.get_size();
    prog.put_lex({ PolizType::LABEL, nullptr });
    
    function_params_init.clear();
    check_function_brace:
    if (current_token.value == ")" || was_parameters) {
        create_function(name, func_type, function_params_pref, function_params_init, ptr);
        current_token = lex.get_token();
        if (current_token.value != "{")
            throw std::exception("Invalid token: '{' expected");
        current_token = lex.get_token();
        while (current_token.value != "}" && current_token.line != -1)
            statement(lex);
        if (current_token.value != "}")
            throw std::exception("Invalid token: '}' expected");
        current_token = lex.get_token();
        prog.put_lex({ PolizType::RET, nullptr });
        may_be_semicolon(lex);
    } else {
        parameter_list(lex);
        was_parameters = true;
        goto check_function_brace;
    }
}

void function_or_var_definitions(LexicalAnalyzer& lex, bool is_program) {
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

void expression(LexicalAnalyzer& lex, bool is_vars, bool is_print) {
    assignment_expression(lex);
    if (is_print)
        prog.put_lex({ PolizType::PRINT, nullptr });

    while (!is_vars && current_token.value == ",") {
        exprs.pop();
        current_token = lex.get_token();
        assignment_expression(lex);
        if (is_print)
            prog.put_lex({ PolizType::PRINT, nullptr });
        if (!is_print)
            prog.put_lex({ PolizType::COMMA, new std::string(",") });
    }
}

bool check_operation(Type f, Type s, std::string operation) {
    auto first = f.expr_type, second = s.expr_type;
    if (operation == ">>" || operation == "<<" || operation == "~" || operation == "|" || operation == "&" || operation == "^" || operation == "->")
        if (first == ExprType::Double || first == ExprType::Float || second == ExprType::Double || second == ExprType::Float)
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

void assignment_expression(LexicalAnalyzer& lex) {
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
        if (!exprs.top().second)
            throw std::exception("Cannot apply assignment operator to rvalue");
        if (exprs.top().first.is_const)
            throw std::exception("Cannot apply assignment operator to const");
        auto prev = exprs.top().first;
        exprs.pop();
        current_token = lex.get_token();
        logical_or_expression(lex);
        operations.push_back(full_operation);
        if (!check_operation(prev, exprs.top().first, operation))
            throw std::exception(("Cannot apply " + tmp_operation + " operator with " + type_to_string(prev) +
                " and " + type_to_string(exprs.top().first)).c_str());
        auto curr = exprs.top();
        exprs.pop();
        exprs.push({ calc_expr_type(prev, curr.first), curr.second });
    }
    for (int i = (int)operations.size() - 1; i >= 0; --i) {
        prog.put_lex({ PolizType::ASSIGN, new std::string(operations[i])});
    }
}

void logical_or_expression(LexicalAnalyzer& lex) {
    logical_and_expression(lex);
    while (current_token.value == "||") {
        current_token = lex.get_token();
        auto prev = exprs.top().first;
        exprs.pop();
        logical_and_expression(lex);
        prog.put_lex({ PolizType::LOGICAL_OR, new std::string("||") });
        if (!check_operation(prev, exprs.top().first, "||"))
            throw std::exception(("Cannot apply || operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
        auto curr = exprs.top();
        exprs.pop();
        exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void logical_and_expression(LexicalAnalyzer& lex) {
    bitwise_or_expression(lex);
    while (current_token.value == "&&") {
        current_token = lex.get_token();
        auto prev = exprs.top().first;
        exprs.pop();
        bitwise_or_expression(lex);
        prog.put_lex({ PolizType::LOGICAL_AND, new std::string("&&") });
        if (!check_operation(prev, exprs.top().first, "&&"))
            throw std::exception(("Cannot apply && operator with " + type_to_string(prev) + " and " + type_to_string(exprs.top().first)).c_str());
        auto curr = exprs.top();
        exprs.pop();
        exprs.push({ Type(ExprType::Bool, false, false), false });
    }
}

void bitwise_or_expression(LexicalAnalyzer& lex) {
    bitwise_xor_expression(lex);
    while (current_token.value == "|") {
        current_token = lex.get_token();
        auto prev = exprs.top().first;
        exprs.pop();
        bitwise_xor_expression(lex);
        prog.put_lex({ PolizType::BITWISE_OR, new std::string("|") });
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
        prog.put_lex({ PolizType::BITWISE_XOR, new std::string("^") });
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
        prog.put_lex({ PolizType::BITWISE_CONS, new std::string("->") });
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
        prog.put_lex({ PolizType::BITWISE_AND, new std::string("&") });
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
        prog.put_lex({ PolizType::EQUALITY, new std::string(operation) });
        if (!check_operation(prev, exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) + 
                " and " + type_to_string(exprs.top().first)).c_str());
        auto curr = exprs.top();
        exprs.pop();
        exprs.push({ Type(ExprType::Bool, false, false), false});
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
        prog.put_lex({ PolizType::CMP, new std::string(operation) });
        if (!check_operation(prev, exprs.top().first, operation))
            throw std::exception(("Cannot apply " + operation + " operator with " + type_to_string(prev) +
                " and " + type_to_string(exprs.top().first)).c_str());
        auto curr = exprs.top();
        exprs.pop();
        exprs.push({ Type(ExprType::Bool, false, false), false });
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
        prog.put_lex({ PolizType::BITWISE_SHIFT, new std::string(operation) });
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
        prog.put_lex({ PolizType::PLUS, new std::string(operation) });
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
        prog.put_lex({ PolizType::MULT, new std::string(operation) });
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
    for (int i = (int)operations.size() - 1; i >= 0; --i) {
        prog.put_lex({ PolizType::UNARY, new std::string(operations[i])});
    }
    
    auto curr = exprs.top();
    exprs.pop();
    for (int i = operations.size() - 1; i >= 0; --i) {
        if (curr.first.is_array)
            throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
        if (operations[i] == "+" || operations[i] == "-" || operations[i] == "~") {
            if (curr.first.expr_type == ExprType::String || curr.first.expr_type == ExprType::Bool || curr.first.expr_type == ExprType::Unknown || 
                curr.first.expr_type == ExprType::Void)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
            curr.second = false;
        } else if (operations[i] == "!") {
            if (curr.first.expr_type != ExprType::Bool)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
            curr.second = false;
        } else {
            if (curr.first.expr_type == ExprType::String || curr.first.expr_type == ExprType::Bool || curr.first.expr_type == ExprType::Unknown || 
                curr.first.expr_type == ExprType::Void)
                throw std::exception(("Cannot apply " + operations[i] + " operator to " + type_to_string(curr.first)).c_str());
            if (!curr.second)
                throw std::exception(("Cannot apply " + operations[i] + " operator to rvalue").c_str());
            if (curr.first.is_const)
                throw std::exception(("Cannot apply " + operations[i] + " operator to const").c_str());
        }
    }
    exprs.push(curr);
}

Type literal_to_type(LexicalAnalyzer& lex) {
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

void construct_expression(LexicalAnalyzer& lex) {
    if (current_token.type == LexicalAnalyzer::token_type::literal) {
        exprs.push({ literal_to_type(lex), false });
        prog.put_lex({ PolizType::LITERAL, new std::pair<std::string, Type>(current_token.value, literal_to_type(lex)) });
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

void field(LexicalAnalyzer& lex, bool only_array) {
    if (current_token.type != LexicalAnalyzer::token_type::identifier)
        throw std::exception("Invalid token: identifier expected");
    std::string name = current_token.value;
    current_token = lex.get_token();
    Identifier* ident = nullptr;
    if (current_token.value == "[")
        ident = get_identifier(name);
    while (current_token.value == "[" || current_token.value == "(" && !only_array) { // TODO: field .
        if (current_token.value == "[") {
            if (!ident->type.is_array && ident->type.expr_type != ExprType::String)
                throw std::exception("Cannot apply operator [] to anything other than an array or string");
            current_token = lex.get_token();

            prog.put_lex({ PolizType::ADDRESS, ident });
            expression(lex, true);
            if (!is_convertible(exprs.top().first, Type(ExprType::Long, false, false)))
                throw std::exception("Cannot access index that is not an integer");
            prog.put_lex({ PolizType::GETARR, new std::string("[]") });
            exprs.pop();
            if (current_token.value != "]")
                throw std::exception("Invalid token: ']' expected");
            current_token = lex.get_token();
            if (current_token.value != "[") {
                exprs.push({ Type(ident->type.is_array ? ident->type.expr_type : ExprType::Char, false, false), true });
            } else if (ident->type.expr_type != ExprType::String)
                throw std::exception("Cannot apply operator [] to anything other than an array or string");
            else {
                current_token = lex.get_token();
                expression(lex, true);
                if (!is_convertible(exprs.top().first, Type(ExprType::Long, false, false)))
                    throw std::exception("Cannot access index that is not an integer");
                prog.put_lex({ PolizType::GETARR, new std::string("[]") });
                exprs.pop();
                if (current_token.value != "]")
                    throw std::exception("Invalid token: ']' expected");
                current_token = lex.get_token();
                exprs.push({ Type(ExprType::Char, false, false), true });
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
                idents.push_back(exprs.top().first);
                exprs.pop();
            }
            while (current_token.value == ",") {
                current_token = lex.get_token();
                expression(lex, true);
                idents.push_back(exprs.top().first);
                exprs.pop();
            }
            if (current_token.value != ")")
                throw std::exception("Invalid token: ')' expected");
            current_token = lex.get_token();

            auto func = get_function(name, idents);
            prog.put_lex({ PolizType::ADDRESS, new Function(func) });
            prog.put_lex({ PolizType::CALL, new int(idents.size()) });

            if (current_token.value == "[") {
                if (!func.type.is_array)
                    throw std::exception("Cannot apply operator [] to anything other than an array");
                current_token = lex.get_token();
                expression(lex, true);
                if (!is_convertible(exprs.top().first, Type(ExprType::Long, false, false)))
                    throw std::exception("Cannot access index that is not an integer");
                exprs.pop();
                if (current_token.value != "]")
                    throw std::exception("Invalid token: ']' expected");
                prog.put_lex({ PolizType::GETARR, new std::string("[]") });
                exprs.push({ Type(func.type.expr_type, false, false), false });
                current_token = lex.get_token();
            }
            else
                exprs.push({ func.type, false });
            return;
        }
    }

    ident = get_identifier(name);
    exprs.push({ ident->type, true });
    prog.put_lex({ PolizType::ADDRESS, ident });
}

void parameter_list(LexicalAnalyzer& lex) {
    is_in_function_header = true;
    was_function_assign = false;
    function_params_pref = 0;

    var_definition(lex);
    prog.blank();
    function_params_init.push_back(prog.get_size() - 1);

    while (current_token.value == ",") {
        current_token = lex.get_token();
        var_definition(lex);
        prog.blank();
        function_params_init.push_back(prog.get_size() - 1);
    }
    is_in_function_header = false;
}

void if_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "if")
        throw std::exception("Invalid token: 'if' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    if (exprs.top().first.expr_type != ExprType::Bool || exprs.top().first.is_array)
        throw std::exception(("Invalid expression type. bool expected, but found " + type_to_string(exprs.top().first)).c_str());
    exprs.pop();
    prog.put_lex({ PolizType::POINTER, nullptr });
    int ptrF = prog.get_size() - 1;
    prog.put_lex({ PolizType::FGO, new std::string("!F") });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    statement(lex);
    int* ptrEnd = new int(-1);
    prog.put_lex({ PolizType::POINTER, ptrEnd });
    prog.put_lex({ PolizType::GO, new std::string("!") });
    prog.blank();
    prog[ptrF].second = new int(prog.get_size() - 1);

    while (current_token.value == "elif") {
        current_token = lex.get_token();

        if (current_token.value != "(")
            throw std::exception("Invalid token: '(' expected");
        current_token = lex.get_token();
        expression(lex);
        if (exprs.top().first.expr_type != ExprType::Bool || exprs.top().first.is_array)
            throw std::exception(("Invalid expression type. bool expected, but found " + type_to_string(exprs.top().first)).c_str());
        exprs.pop();
        prog.put_lex({ PolizType::POINTER, nullptr });
        ptrF = prog.get_size() - 1;
        prog.put_lex({ PolizType::FGO, new std::string("!F") });

        if (current_token.value != ")")
            throw std::exception("Invalid token: ')' expected");
        current_token = lex.get_token();
        statement(lex);

        prog.put_lex({ PolizType::POINTER, ptrEnd });
        prog.put_lex({ PolizType::GO, new std::string("!") });
        prog.blank();
        prog[ptrF].second = new int(prog.get_size() - 1);
    }
    if (current_token.value == "else") {
        current_token = lex.get_token();
        statement(lex);
    }

    *ptrEnd = prog.get_size() - 1;
}

void goto_statement(LexicalAnalyzer& lex) {
    if (current_token.value == "break") {
        if (break_ptr.empty())
            throw std::exception("It is not possible to call break in the current context");
        current_token = lex.get_token();
        prog.put_lex({ PolizType::POINTER, break_ptr.top() });
        prog.put_lex({ PolizType::GO, new std::string("!") });
    } else if (current_token.value == "continue") {
        if (continue_ptr.empty())
            throw std::exception("It is not possible to call continue in the current context");
        current_token = lex.get_token();
        prog.put_lex({ PolizType::POINTER, continue_ptr.top() });
        prog.put_lex({ PolizType::GO, new std::string("!") });
    } /*else if (current_token.value == "goto") { TODO: add goto
        current_token = lex.get_token();
        if (current_token.type != LexicalAnalyzer::token_type::identifier)
            throw std::exception("Invalid token: 'identifier' expected");
        current_token = lex.get_token();
    } */else
        throw std::exception("Invalid token: 'break', 'continue' or 'goto' expected");
}

void return_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "return")
        throw std::exception("Invalid token: 'return' expected");
    current_token = lex.get_token();
    if (current_token.value != ";") {
        expression(lex);
        auto type = exprs.top().first;
        exprs.pop();
        if (!is_convertible(type, current_function))
            throw std::exception(("Cannot cast " + type_to_string(type) + " type to " + type_to_string(current_function)).c_str());
    } else {
        if (!is_convertible(Type(ExprType::Void, false, false), current_function))
            throw std::exception("The function requires a return value.");
    }
    prog.put_lex({ PolizType::RET, nullptr });
}

void while_statement(LexicalAnalyzer& lex) {
    prog.blank();
    int* ptrBeg = new int(prog.get_size() - 1);
    int* ptrEnd = new int(-1);
    continue_ptr.push(ptrBeg);
    break_ptr.push(ptrEnd);
    if (current_token.value != "while")
        throw std::exception("Invalid token: 'while' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    if (exprs.top().first.expr_type != ExprType::Bool || exprs.top().first.is_array)
        throw std::exception(("Invalid expression type. bool expected, but found " + type_to_string(exprs.top().first)).c_str());
    exprs.pop();
    prog.put_lex({ PolizType::POINTER, ptrEnd });
    prog.put_lex({ PolizType::FGO, new std::string("!F") });

    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    statement(lex);

    prog.put_lex({ PolizType::POINTER, ptrBeg });
    prog.put_lex({ PolizType::GO, new std::string("!") });

    prog.blank();
    *ptrEnd = prog.get_size() - 1;

    continue_ptr.pop();
    break_ptr.pop();
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
                    exprs.pop();
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
    break_ptr.push(ptrEnd);
    continue_ptr.push(ptrAct);

    if (current_token.value != ";") {
        expression(lex);
        if (exprs.top().first.expr_type != ExprType::Bool || exprs.top().first.is_array)
            throw std::exception(("Invalid expression type. bool expected, but found " + type_to_string(exprs.top().first)).c_str());
        exprs.pop();
        
        prog.put_lex({ PolizType::POINTER, ptrEnd });
        prog.put_lex({ PolizType::FGO, new std::string("!F") });

        prog.put_lex({ PolizType::POINTER, ptrBody });
        prog.put_lex({ PolizType::GO, new std::string("!") });
    }
    semicolon(lex, true);
    prog.blank();
    *ptrAct = prog.get_size() - 1;
    if (current_token.value != ")") {
        expression(lex);
        exprs.pop();
    }
    prog.put_lex({ PolizType::POINTER, ptrExpr });
    prog.put_lex({ PolizType::GO, new std::string("!") });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    prog.blank();
    *ptrBody = prog.get_size() - 1;
    statement(lex, true);
    prog.put_lex({ PolizType::POINTER, ptrAct });
    prog.put_lex({ PolizType::GO, new std::string("!") });
    prog.blank();
    *ptrEnd = prog.get_size() - 1;
    break_ptr.pop();
    continue_ptr.pop();
}

void switch_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "switch")
        throw std::exception("Invalid token: 'switch' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex);
    Type curr_type = exprs.top().first;
    exprs.pop();
    std::unordered_set<std::string> cases;
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
    if (current_token.value != "{")
        throw std::exception("Invalid token: '{' expected");
    current_token = lex.get_token();
    int* ptr_end = new int(-1);
    prog.put_lex({ PolizType::STACK_PLUG, nullptr });

    while (current_token.value == "case" || current_token.value == "default") {
        break_ptr.push(ptr_end);
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
            if (!is_convertible(literal_to_type(lex), curr_type))
                throw std::exception(("Cannot cast " + type_to_string(literal_to_type(lex)) + " type to " + type_to_string(curr_type)).c_str());

            cases.insert(val);
            current_token = lex.get_token();
            if (current_token.value != ":")
                throw std::exception("Invalid token: ':' expected");
            current_token = lex.get_token();

            int* ptr_case_end = new int(-1);
            prog.put_lex({ PolizType::LITERAL, new std::pair<std::string, ExprType>(val, curr_type.expr_type) });
            prog.put_lex({ PolizType::SWITCH_CMP, new std::string("==") });
            prog.put_lex({ PolizType::POINTER, ptr_case_end });
            prog.put_lex({ PolizType::FGO, new std::string("!F")});

            statement(lex);
            prog.blank();
            *ptr_case_end = prog.get_size() - 1;
        }
        break_ptr.pop();
    }
    if (current_token.value != "}")
        throw std::exception("Invalid token: '}' expected");
    current_token = lex.get_token();

    prog.put_lex({ PolizType::SWITCH_POP, new std::string("POP") });
    *ptr_end = prog.get_size() - 1;
}

void print_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "print")
        throw std::exception("Invalid token: 'print' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    expression(lex, false, true);
    exprs.pop();
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void read_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "read")
        throw std::exception("Invalid token: 'read' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    field(lex, true);
    exprs.pop();
    prog.put_lex({ PolizType::READ, nullptr });
    while (current_token.value == ",") {
        current_token = lex.get_token();
        field(lex, true);
        exprs.pop();
        prog.put_lex({ PolizType::READ, nullptr });
    }
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
    current_token = lex.get_token();
}

void readln_statement(LexicalAnalyzer& lex) {
    if (current_token.value != "readln")
        throw std::exception("Invalid token: 'readln' expected");
    current_token = lex.get_token();
    if (current_token.value != "(")
        throw std::exception("Invalid token: '(' expected");
    current_token = lex.get_token();
    field(lex, true);
    auto type = exprs.top().first;
    if (type.expr_type != ExprType::String)
        throw std::exception("Cannot use readln with not a string identifier");
    exprs.pop();
    prog.put_lex({ PolizType::READLN, nullptr });
    if (current_token.value != ")")
        throw std::exception("Invalid token: ')' expected");
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
            exprs.pop();
            semicolon(lex);
            may_be_semicolon(lex);
        } else {
            may_be_semicolon(lex);
        }
    }
}

int main(int argc, char* argv[]) {
    std::string path;
    if (argc != 2) {
        /*std::cin >> path;
        if (path[0] == '\"') {
            path = path.substr(1, path.size() - 2);
        }*/
        path = "C:\\Users\\sasha\\Documents\\test.prog";
    } else {
        path = argv[1];
    }
    
    LexicalAnalyzer lex(path);

    printf("\x1B[32mThe lexical analysis was successful\n\033[0m");

    try {
        current_token = lex.get_token();
        program(lex);

        if (current_token.line != -1) {
            printf("\x1B[31mUnexpected end of file\n\033[0m");
            exit(0);
        }
        printf("\x1B[32mThe syntactic & semantic analysis was successful\n\033[0m");
        printf("\x1B[32mThe internal representation of the program has been successfully generated\n\033[0m");
        auto main = get_function("main", std::vector<Type>());
        printf("Main ptr: %d\n", main.ptr);
        prog.execute(main.ptr);
    } catch (std::exception ex) {
        printf(("\x1B[31m" + std::string(ex.what())).c_str());
        if (current_token.line != -1) {
            std::cout << " (" << current_token.line << " line)";
        }
        printf("\033[0m");
    }
}