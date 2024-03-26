#include "Poliz.h"

ExprType Poliz::string_to_type(std::string str) {
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

Poliz::Poliz(Type* current_function_) {
    rnd = std::mt19937((std::chrono::high_resolution_clock::now().time_since_epoch().count() + 13));
    current_function = current_function_;
}
Poliz::~Poliz() {
    for (int i = 0; i < lexes.size(); ++i) {
        if (lexes[i].first != Poliz::PolizType::ADDRESS && lexes[i].first != Poliz::PolizType::POINTER && lexes[i].second != nullptr)
            delete lexes[i].second;
    }
}
void Poliz::put_lex(std::pair<Poliz::PolizType, void*> l) {
    if (*current_function != Type())
        lexes.push_back(l);
    else
        global_lexes.push_back(l);
}
void Poliz::blank() {
    lexes.push_back({ Poliz::PolizType::BLANK, nullptr });
}
int Poliz::get_size() {
    return lexes.size();
}
void* Poliz::convert(std::pair<Poliz::PolizType, void*> op, Poliz::PolizType to) {
    if (op.first == Poliz::PolizType::ADDRESS)
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
        case LARGE_LITERAL:
            return new large(*((bool*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((uint8_t*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((char*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((double*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((float*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((int*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((long long*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((short*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((std::string*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((uint32_t*)op.second));
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
        case LARGE_LITERAL:
            return new large(*((uint64_t*)op.second));
        }
        break;
    }
    case LARGE_LITERAL: {
        switch (to) {
        case BOOL_LITERAL:
            return new bool(std::stoll(((large*)op.second)->ToString()));
            break;
        case BYTE_LITERAL:
            return new uint8_t(std::stoll(((large*)op.second)->ToString()));
            break;
        case CHAR_LITERAL:
            return new char(std::stoll(((large*)op.second)->ToString()));
            break;
        case DOUBLE_LITERAL:
            return new double(std::stoll(((large*)op.second)->ToString()));
            break;
        case FLOAT_LITERAL:
            return new float(std::stoll(((large*)op.second)->ToString()));
            break;
        case INT_LITERAL:
            return new int(std::stoll(((large*)op.second)->ToString()));
            break;
        case LONG_LITERAL:
            return new long long(std::stoll(((large*)op.second)->ToString()));
            break;
        case SHORT_LITERAL:
            return new short(std::stoll(((large*)op.second)->ToString()));
            break;
        case STRING_LITERAL:
            return new std::string(((large*)op.second)->ToString());
            break;
        case UINT_LITERAL:
            return new uint32_t(std::stoll(((large*)op.second)->ToString()));
            break;
        case ULONG_LITERAL:
            return new uint64_t(std::stoll(((large*)op.second)->ToString()));
            break;
        case LARGE_LITERAL:
            return new large(*((large*)op.second));
        }
        break;
    }
    }
    throw std::exception("Cannot convert types");
}
std::pair<Poliz::PolizType, std::pair<void*, void*>> Poliz::make_same(std::pair<Poliz::PolizType, void*> op1, std::pair<Poliz::PolizType, void*> op2) {
    int pr1 = literal_prior[(int)op1.first - (int)Poliz::PolizType::BOOL_LITERAL], pr2 = literal_prior[(int)op2.first - (int)Poliz::PolizType::BOOL_LITERAL];
    if (pr1 >= pr2)
        return std::pair<Poliz::PolizType, std::pair<void*, void*>>(op1.first, { op1.second, convert(op2, op1.first) });
    else
        return std::pair<Poliz::PolizType, std::pair<void*, void*>>(op2.first, { convert(op1, op2.first), op2.second });

}
std::pair<Poliz::PolizType, void*> Poliz::address_to_value(void* ptr) {
    Identifier* ident = (Identifier*)ptr;
    if (ident->value == nullptr) {
        throw std::exception(("Variable '" + ident->name + "' was not initialized").c_str());
    }
    switch (ident->type.expr_type) {
    case ExprType::Bool:
        return { Poliz::PolizType::BOOL_LITERAL, (bool*)(ident->value) };
    case ExprType::Byte:
        return { Poliz::PolizType::BYTE_LITERAL, (uint8_t*)(ident->value) };
    case ExprType::Char:
        return { Poliz::PolizType::CHAR_LITERAL, (char*)(ident->value) };
    case ExprType::Double:
        return { Poliz::PolizType::DOUBLE_LITERAL, (double*)(ident->value) };
    case ExprType::Float:
        return { Poliz::PolizType::FLOAT_LITERAL, (float*)(ident->value) };
    case ExprType::Int:
        return { Poliz::PolizType::INT_LITERAL, (int*)(ident->value) };
    case ExprType::Long:
        return { Poliz::PolizType::LONG_LITERAL, (long long*)(ident->value) };
    case ExprType::Short:
        return { Poliz::PolizType::SHORT_LITERAL, (short*)(ident->value) };
    case ExprType::String:
        return { Poliz::PolizType::STRING_LITERAL, (std::string*)(ident->value) };
    case ExprType::UInt:
        return { Poliz::PolizType::UINT_LITERAL, (uint32_t*)(ident->value) };
    case ExprType::ULong:
        return { Poliz::PolizType::ULONG_LITERAL, (uint64_t*)(ident->value) };
    case ExprType::Large:
        return { Poliz::PolizType::LARGE_LITERAL, (large*)(ident->value) };
    default:
        throw std::exception(("Variable '" + ident->name + "' has unconvertable value").c_str());
    }
}
Poliz::PolizType Poliz::type_to_poliz(ExprType type) {
    switch (type) {
    case Bool:
        return Poliz::PolizType::BOOL_LITERAL;
    case Char:
        return Poliz::PolizType::CHAR_LITERAL;
    case Double:
        return Poliz::PolizType::DOUBLE_LITERAL;
    case Float:
        return Poliz::PolizType::FLOAT_LITERAL;
    case Int:
        return Poliz::PolizType::INT_LITERAL;
    case UInt:
        return Poliz::PolizType::UINT_LITERAL;
    case Long:
        return Poliz::PolizType::LONG_LITERAL;
    case ULong:
        return Poliz::PolizType::ULONG_LITERAL;
    case Short:
        return Poliz::PolizType::SHORT_LITERAL;
    case Byte:
        return Poliz::PolizType::BYTE_LITERAL;
    case String:
        return Poliz::PolizType::STRING_LITERAL;
    case Large:
        return Poliz::PolizType::LARGE_LITERAL;
    default:
        return Poliz::PolizType::BLANK;
    }
}
std::pair<Poliz::PolizType, void*> Poliz::execute_operation(std::pair<Poliz::PolizType, void*> op1, std::pair<Poliz::PolizType, void*> op2, std::string operation) {
    if (op1.first == Poliz::PolizType::ADDRESS)
        op1 = address_to_value(op1.second);
    if (op2.first == Poliz::PolizType::ADDRESS)
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) + *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "-") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) - *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "*") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) * *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "/") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) / *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "%") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) % *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == ">>") {
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
    } else if (operation == "<<") {
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
    } else if (operation == "<") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) < *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "<=") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) <= *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == ">") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) > *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == ">=") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) >= *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "==") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) == *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "!=") {
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
        case LARGE_LITERAL:
            return { conv.first, new large(*((large*)conv.second.first) != *((large*)conv.second.second)) };
        default:
            throw std::exception("Cannot use this operation");
        }
    } else if (operation == "&") {
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
    } else if (operation == "|") {
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
    } else if (operation == "^") {
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
    } else if (operation == "->") {
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
    } else if (operation == "&&") {
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
    } else if (operation == "||") {
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
std::pair<Poliz::PolizType, void*> Poliz::execute_operation(std::pair<Poliz::PolizType, void*> op, std::string operation) {
    Identifier* ident = nullptr;
    if (op.first == Poliz::PolizType::ADDRESS) {
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
        case LARGE_LITERAL: {
            auto val = *((large*)op.second);
            ++val;
            ident->value = new large(val);
            return { ADDRESS, ident };
        }
        }
    } else if (operation == "--") {
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
        case LARGE_LITERAL: {
            auto val = *((large*)op.second);
            --val;
            ident->value = new large(val);
            return { ADDRESS, ident };
        }
        }
    } else if (operation == "+") {
        return op;
    } else if (operation == "-") {
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
        case LARGE_LITERAL: {
            return { op.first, new large(large(0) - *((large*)op.second)) };
        }
        }
    } else if (operation == "!") {
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
    } else if (operation == "~") {
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
void Poliz::call_function(std::stack<std::pair<Poliz::PolizType, void*>>& st, int& p) {
    int sz = *(int*)st.top().second;
    st.pop();
    Function* func = (Function*)st.top().second;
    st.pop();
    p = func->ptr;
    std::stack<std::pair<Poliz::PolizType, void*>> args;
    for (int i = 0; i < sz; ++i) {
        args.push(st.top());
        st.pop();
    }
    st.push({ Poliz::PolizType::STACK_PLUG, nullptr });
    int q = 0;
    while (!args.empty()) {
        func->params_ptrs[q]->value = convert(args.top(), type_to_poliz(func->params_ptrs[q]->type.expr_type));
        args.pop();
        p = func->params_init[q];
        ++q;
    }
}
std::pair<Poliz::PolizType, void*> Poliz::execute(Function entry_func) {
    std::stack<std::pair<Poliz::PolizType, void*>> st;
    int p = 0;
    int label_balance = 0;
    st.push({ Poliz::PolizType::STACK_PLUG, nullptr });
    func_calls.push({ lexes.size(), type_to_poliz(entry_func.type.expr_type) });

    while (p < global_lexes.size()) {
        switch (global_lexes[p].first) {
        case BLANK: {
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
        case LARGE_LITERAL:
        case POINTER: {
            st.push(global_lexes[p]);
            ++p;
            break;
        }
        case GETARR: {
            int ind = *(int*)convert(st.top(), Poliz::PolizType::INT_LITERAL);
            st.pop();
            if (st.top().second == nullptr)
                throw std::exception("Cannot access the element of an uninitialized array");
            auto var = (std::vector<void*>*)((Identifier*)st.top().second)->value;
            st.pop();
            if (var->size() <= ind || ind < 0)
                throw std::exception("Array index out of range");
            st.push({ Poliz::PolizType::ADDRESS, var->operator[](ind) });
            ++p;
            break;
        }
        case GETSTR: {
            int ind = *(int*)convert(st.top(), Poliz::PolizType::INT_LITERAL);
            st.pop();
            if (st.top().second == nullptr)
                throw std::exception("Cannot access the character of an uninitialized string");
            auto var = (std::string*)((Identifier*)st.top().second)->value;
            auto name = ((Identifier*)st.top().second)->name;
            st.pop();
            if (var->size() <= ind || ind < 0)
                throw std::exception("String index out of range");
            auto ptr = new Identifier(name + "[" + std::to_string(ind) + "]", Type(ExprType::Char, false, false), nullptr);
            ptr->value = &(var->operator[](ind));
            st.push({ Poliz::PolizType::ADDRESS, ptr });
            ++p;
            break;
        }
        case SEMICOLON: {
            while (!st.empty() && st.top().first != Poliz::PolizType::STACK_PLUG) {
                st.pop();
            }
            ++p;
            break;
        }
        case LITERAL: {
            std::pair<std::string, Type> val = *((std::pair<std::string, Type>*)global_lexes[p].second);
            if (val.second.expr_type == ExprType::Bool) {
                st.push({ Poliz::PolizType::BOOL_LITERAL, new bool(val.first == "true") });
            } else if (val.second.expr_type == ExprType::Byte || val.second.expr_type == ExprType::UShort) {
                st.push({ Poliz::PolizType::BYTE_LITERAL, new uint8_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Char) {
                std::string s = val.first.substr(1, val.first.size() - 2);
                if (s == "\\\'")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\'') });
                else if (s == "\\\"")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\"') });
                else if (s == "\\n")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\n') });
                else if (s == "\\t")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\t') });
                else if (s == "\\0")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\0') });
                else
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char(s[0]) });
            } else if (val.second.expr_type == ExprType::Double) {
                st.push({ Poliz::PolizType::DOUBLE_LITERAL, new double(std::stold(val.first)) });
            } else if (val.second.expr_type == ExprType::Float) {
                st.push({ Poliz::PolizType::FLOAT_LITERAL, new float(std::stold(val.first)) });
            } else if (val.second.expr_type == ExprType::Int) {
                st.push({ Poliz::PolizType::INT_LITERAL, new int(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Long) {
                st.push({ Poliz::PolizType::LONG_LITERAL, new long long(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Short) {
                st.push({ Poliz::PolizType::SHORT_LITERAL, new short(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::String) {
                st.push({ Poliz::PolizType::STRING_LITERAL, new std::string(val.first.substr(1, val.first.size() - 2)) });
            } else if (val.second.expr_type == ExprType::UInt) {
                st.push({ Poliz::PolizType::UINT_LITERAL, new uint32_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::ULong) {
                st.push({ Poliz::PolizType::ULONG_LITERAL, new uint64_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Large) {
                st.push({ Poliz::PolizType::LARGE_LITERAL, new large(val.first) });
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
            if (op2.first == Poliz::PolizType::ADDRESS) {
                assign_val = ((Identifier*)op2.second)->value;
                if (assign_val == nullptr)
                    throw std::exception(("Cannot use a variable '" + ((Identifier*)op2.second)->name + "' that has not been assigned a value").c_str());
            }

            if (*((std::string*)global_lexes[p].second) == "=") {
                if (var->type.is_array)
                    var->value = ((Identifier*)op2.second)->value;
                else if (var->type.expr_type == ExprType::Char && *var->name.rbegin() == ']')
                    *(char*)var->value = *(char*)convert(op2, type_to_poliz(var->type.expr_type));
                else
                    var->value = convert(op2, type_to_poliz(var->type.expr_type));
            } else {
                std::string op = *((std::string*)global_lexes[p].second);
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
            st.push(execute_operation(op, *((std::string*)global_lexes[p].second)));
            ++p;
            break;
        }
        case CONVERT: {
            throw std::exception("convert() function is restricted in the global scope");
        }
        case RAND: {
            throw std::exception("rand() function is restricted in the global scope");
        }
        case STRLEN: {
            throw std::exception("strlen() function is restricted in the global scope");
        }
        case CALL: {
            throw std::exception("function calls are restricted in the global scope");
        }
        case TIME: {
            throw std::exception("time() function is restricted in the global scope");
        }
        default:
            auto op2 = st.top();
            st.pop();
            auto op1 = st.top();
            st.pop();
            st.push(execute_operation(op1, op2, *(std::string*)(global_lexes[p].second)));
            ++p;
            break;
        }
    }

    while (st.size() > 1)
        st.pop();

    label_balance = 0;
    p = entry_func.ptr;

    while (p < lexes.size()) {
        if (func_calls.size() > 200000) throw std::exception("Stack overflow");

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
        case BLANK: {
            ++p;
            break;
        }
        case LABEL: {
            ++label_balance;
            ++p;
            if (func_calls.size() != label_balance)
                throw std::exception("The return statement was not called");
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
        case LARGE_LITERAL:
        case POINTER: {
            st.push(lexes[p]);
            ++p;
            break;
        }
        case GETARR: {
            int ind = *(int*)convert(st.top(), Poliz::PolizType::INT_LITERAL);
            st.pop();
            if (st.top().second == nullptr)
                throw std::exception("Cannot access the element of an uninitialized array");
            auto var = (std::vector<void*>*)((Identifier*)st.top().second)->value;
            st.pop();
            if (var->size() <= ind)
                throw std::exception("Array index out of range");
            st.push({ Poliz::PolizType::ADDRESS, var->operator[](ind) });
            ++p;
            break;
        }
        case GETSTR: {
            int ind = *(int*)convert(st.top(), Poliz::PolizType::INT_LITERAL);
            st.pop();
            if (st.top().second == nullptr)
                throw std::exception("Cannot access the character of an uninitialized string");
            auto var = (std::string*)((Identifier*)st.top().second)->value;
            auto name = ((Identifier*)st.top().second)->name;
            st.pop();
            if (var->size() <= ind)
                throw std::exception("String index out of range");
            auto ptr = new Identifier(name + "[" + std::to_string(ind) + "]", Type(ExprType::Char, false, false), nullptr);
            ptr->value = &(var->operator[](ind));
            st.push({ Poliz::PolizType::ADDRESS, ptr });
            ++p;
            break;
        }
        case SEMICOLON: {
            while (!st.empty() && st.top().first != Poliz::PolizType::STACK_PLUG) {
                st.pop();
            }
            ++p;
            break;
        }
        case LITERAL: {
            std::pair<std::string, Type> val = *((std::pair<std::string, Type>*)lexes[p].second);
            if (val.second.expr_type == ExprType::Bool) {
                st.push({ Poliz::PolizType::BOOL_LITERAL, new bool(val.first == "true") });
            } else if (val.second.expr_type == ExprType::Byte || val.second.expr_type == ExprType::UShort) {
                st.push({ Poliz::PolizType::BYTE_LITERAL, new uint8_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Char) {
                std::string s = val.first.substr(1, val.first.size() - 2);
                if (s == "\\\'")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\'') });
                else if (s == "\\\"")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\"') });
                else if (s == "\\n")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\n') });
                else if (s == "\\t")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\t') });
                else if (s == "\\0")
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char('\0') });
                else
                    st.push({ Poliz::PolizType::CHAR_LITERAL, new char(s[0]) });
            } else if (val.second.expr_type == ExprType::Double) {
                st.push({ Poliz::PolizType::DOUBLE_LITERAL, new double(std::stold(val.first)) });
            } else if (val.second.expr_type == ExprType::Float) {
                st.push({ Poliz::PolizType::FLOAT_LITERAL, new float(std::stold(val.first)) });
            } else if (val.second.expr_type == ExprType::Int) {
                st.push({ Poliz::PolizType::INT_LITERAL, new int(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Long) {
                st.push({ Poliz::PolizType::LONG_LITERAL, new long long(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Short) {
                st.push({ Poliz::PolizType::SHORT_LITERAL, new short(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::String) {
                st.push({ Poliz::PolizType::STRING_LITERAL, new std::string(val.first.substr(1, val.first.size() - 2)) });
            } else if (val.second.expr_type == ExprType::UInt) {
                st.push({ Poliz::PolizType::UINT_LITERAL, new uint32_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::ULong) {
                st.push({ Poliz::PolizType::ULONG_LITERAL, new uint64_t(std::stoll(val.first)) });
            } else if (val.second.expr_type == ExprType::Large) {
                st.push({ Poliz::PolizType::LARGE_LITERAL, new large(val.first) });
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
            if (op2.first == Poliz::PolizType::ADDRESS) {
                assign_val = ((Identifier*)op2.second)->value;
                if (assign_val == nullptr)
                    throw std::exception(("Cannot use a variable '" + ((Identifier*)op2.second)->name + "' that has not been assigned a value").c_str());
            }

            if (*((std::string*)lexes[p].second) == "=") {
                if (var->type.is_array)
                    var->value = ((Identifier*)op2.second)->value;
                else if (var->type.expr_type == ExprType::Char && *var->name.rbegin() == ']')
                    *(char*)var->value = *(char*)convert(op2, type_to_poliz(var->type.expr_type));
                else
                    var->value = convert(op2, type_to_poliz(var->type.expr_type));
            } else {
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
            auto top = st.top();
            while (st.top().first != Poliz::PolizType::STACK_PLUG) {
                st.pop();
            }
            st.pop();
            if (top.first == Poliz::PolizType::ADDRESS)
                top = address_to_value(top.second);
            top.second = convert(top, func_calls.top().second);
            top.first = func_calls.top().second;
            st.push(top);
            p = func_calls.top().first + 1;
            func_calls.pop();
            --label_balance;
            break;
        }
        case NORET: {
            while (st.top().first != Poliz::PolizType::STACK_PLUG) {
                st.pop();
            }
            st.pop();
            p = func_calls.top().first + 1;
            func_calls.pop();
            --label_balance;
            break;
        }
        case CALL: {
            func_calls.push({ p, type_to_poliz(((Function*)st.top().second)->type.expr_type) });
            st.push(lexes[p]);
            call_function(st, p);
            break;
        }
        case EXIT: {
            return st.top();
        }
        case SWITCH_CMP: {
            auto op2 = st.top();
            st.pop();
            st.pop();
            auto op1 = st.top();
            st.pop();
            st.push(op1);
            st.push({ Poliz::PolizType::STACK_PLUG, nullptr });
            st.push(execute_operation(op1, op2, "=="));
            ++p;
            break;
        }
        case SWITCH_POP: {
            while (st.top().first != Poliz::PolizType::STACK_PLUG) {
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
            var->value = convert({ Poliz::PolizType::STRING_LITERAL, new std::string(value) }, type_to_poliz(var->type.expr_type));
            ++p;
            break;
        }
        case READLN: {
            auto var = (Identifier*)st.top().second;
            st.pop();
            std::string value;
            std::cin.clear();
            std::getline(std::cin, value);
            var->value = convert({ Poliz::PolizType::STRING_LITERAL, new std::string(value) }, type_to_poliz(var->type.expr_type));
            ++p;
            break;
        }
        case PRINT: {
            auto op = st.top();
            st.pop();
            std::cout << *(std::string*)(convert(op, Poliz::PolizType::STRING_LITERAL));
            ++p;
            break;
        }
        case CONVERT: {
            auto type = lexes[p];
            auto val = st.top();
            st.pop();
            auto poliz_type = type_to_poliz(string_to_type(*(std::string*)type.second));
            st.push({ poliz_type, convert(val, poliz_type) });
            ++p;
            break;
        }
        case RAND: {
            st.push({ Poliz::PolizType::INT_LITERAL, new int(rnd()) });
            ++p;
            break;
        }
        case STRLEN: {
            auto op = st.top();
            st.pop();
            auto conv = convert(op, Poliz::PolizType::STRING_LITERAL);
            st.push({ Poliz::PolizType::INT_LITERAL, new int(((std::string*)conv)->size()) });
            ++p;
            break;
        }
        case TIME: {
            const auto p1 = std::chrono::system_clock::now();
            st.push({ Poliz::PolizType::LONG_LITERAL, new long long(std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count()) });
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

    if (!func_calls.empty())
        throw std::exception("The return statement was not called");
    return st.empty() ? std::pair<Poliz::PolizType, void*>(Poliz::PolizType::BLANK, nullptr) : st.top();
}