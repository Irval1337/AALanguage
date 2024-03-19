#pragma once

#include <vector>
#include <unordered_map>
#include <string>

enum ExprType {
	Bool, Char, Double, UDouble, Float, UFloat, Int, UInt, Long, ULong, Short, UShort, Byte, String, Void, Unknown
};

int type_prior[16] = { 0, 1, 11, 12, 7, 8, 5, 6, 9, 10, 3, 4, 2, 0, 0, 0 };

struct Type {
	ExprType expr_type;
	bool is_const;
	bool is_array;
	int array_size;
	Type(ExprType expr_type_, bool is_const_, bool is_array_, int array_size_ = -1) {
		expr_type = expr_type_;
		is_const = is_const_;
		is_array = is_array_;
		array_size = array_size_;
	}
	Type() {
		expr_type = ExprType::Unknown;
		is_const = is_array = false;
		array_size = -1;
	}
	bool operator==(const Type& other) const {
		return expr_type == other.expr_type && is_const == other.is_const && is_array == other.is_array && array_size == other.array_size;
	}
};

struct Identifier {
	std::string name;
	Type type;
	void* value;
	Identifier(std::string name_, Type type_, void* value_ = nullptr) {
		name = name_;
		type = type_;
		value = value_;
	}
	bool operator==(const Identifier& other) const {
		return type == other.type;
	}
};

struct TableIdentifiers {
	TableIdentifiers* parent = nullptr;
	std::vector<TableIdentifiers*> children;
	std::unordered_map<std::string, Identifier*> identifiers;
	~TableIdentifiers() {
		for (auto& u : children) {
			if (!children.empty()) break;
			if (u != nullptr)
				delete u;
			u = nullptr;
		}
		for (auto& u : identifiers) {
			if (!identifiers.empty()) break;
			if (u.second != nullptr)
				delete u.second;
			u.second = nullptr;
		}
	}
};

struct Function {
	std::string name;
	int not_default_pref = 0;
	Type type;
	std::vector<Type> identifiers;
	std::vector<int> params_init;
	std::vector<Identifier*> params_ptrs;
	int ptr;
	Function(std::string name_, Type type_, std::vector<Type> identifiers_, std::vector<int> params_init_, std::vector<Identifier*> params_ptrs_,
		int ptr_, int not_default_pref_ = 0) {
		name = name_;
		type = type_;
		identifiers = identifiers_;
		not_default_pref = not_default_pref_;
		ptr = ptr_;
		params_init = params_init_;
		params_ptrs = params_ptrs_;
	}
	bool operator==(const Function& f) const {
		if (identifiers.size() != f.identifiers.size()) return false;
		bool flag = true;
		for (int i = 0; i < identifiers.size(); ++i) {
			flag &= identifiers[i].expr_type == f.identifiers[i].expr_type && identifiers[i].is_array == f.identifiers[i].is_array;
		}
		
		return name == f.name && flag;
	}
};

class FunctionHasher {
public:
	std::size_t operator()(const Function& f) const {
		std::size_t seed = f.identifiers.size();
		for (auto& i : f.identifiers) {
			seed ^= std::hash<int>{}((int)i.expr_type  + (i.is_array << 8) + (i.array_size << 10)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return std::hash<std::string>{}(f.name) ^ (seed + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}
};