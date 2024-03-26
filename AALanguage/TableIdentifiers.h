#pragma once

#include <vector>
#include <unordered_map>
#include <string>


/**
* @brief Enumeration of all possible value types in the language.
* 
* @deprecated UDouble, UFloat due to redundancy.
*/
enum ExprType {
	Bool, Char, Double, UDouble, Float, UFloat, Int, UInt, Long, ULong, Short, UShort, Byte, String, Void, Large, Unknown
};

/**
* @brief Priorities for data type conversions.
* 
* - 0 - special priority or basic for a group of types.
* - Type A is brought to type B by expansion if A's priority is higher than B's.
*/
static int type_prior[17] = { 0, 1, 11, 12, 7, 8, 5, 6, 9, 10, 3, 4, 2, 0, 0, 13, 0 };

/**
* @brief A class to represent an expression type.
*/
struct Type {
	//! Standard expression type
	ExprType expr_type;
	//! Whether the expression is constant (in case it is a variable)
	bool is_const;
	//! Whether the expression is an array
	bool is_array;
	//! The size of the array or -1 if the type is not an array
	int array_size;
	/**
	* @brief A constructor that specifies the public fields of the type.
	*/
	Type(ExprType expr_type_, bool is_const_, bool is_array_, int array_size_ = -1) {
		expr_type = expr_type_;
		is_const = is_const_;
		is_array = is_array_;
		array_size = array_size_;
	}
	/**
	* @brief Default Constructor.
	* 
	* Sets:
	* - expr_type = Unknown
	* - is_const = is_array = false
	* - array_size = -1.
	*/
	Type() {
		expr_type = ExprType::Unknown;
		is_const = is_array = false;
		array_size = -1;
	}
	/**
	* @brief Type comparison operator.
	* 
	* Compares all public attributes.
	* 
	* @param other Another type to compare. 
	*/
	bool operator==(const Type& other) const {
		return expr_type == other.expr_type && is_const == other.is_const && is_array == other.is_array && array_size == other.array_size;
	}
	/**
	* @brief Type comparison operator (not equality).
	*
	* Compares all public attributes.
	*
	* @param other Another type to compare.
	*/
	bool operator!=(const Type& other) const {
		return !(*this == other);
	}
};

/**
* @brief A class to represent a variable.
*/
struct Identifier {
	//! Variable name
	std::string name;
	//! Standart expression type
	Type type;
	//! Pointer to a value stored in the executor memory
	void* value;
	/**
	* @brief A constructor that specifies the public fields of the type.
	*/
	Identifier(std::string name_, Type type_, void* value_ = nullptr) {
		name = name_;
		type = type_;
		value = value_;
	}
	/**
	* @brief Type comparison operator.
	*
	* Compares all public attributes.
	*
	* @param other Another identifier to compare.
	*/
	bool operator==(const Identifier& other) const {
		return type == other.type;
	}
};

/**
* @brief Identifier table used for semantic analysis.
*/
struct TableIdentifiers {
	//! Pointer to the parent scope identifier table
	TableIdentifiers* parent = nullptr;
	//! Vector-array of pointers to scope child identifier tables
	std::vector<TableIdentifiers*> children;
	//! Hash table of identifiers of the current scope. (key, value) = (name, pointer to Identifier)
	std::unordered_map<std::string, Identifier*> identifiers;
	/**
	* @brief Class destructor.
	* 
	* The destructor recursively calls the child ID table destructors, then deletes all variables and frees their memory.
	*/
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

/**
* @brief A class to represent a function in semantic.
*/
struct Function {
	//! Function name
	std::string name;
	//! Function parameter prefix without default value.
	int not_default_pref = 0;
	//! Return type of the function.
	Type type;
	//! Identifiers types for function parameters.
	std::vector<Type> identifiers;
	//! Initialization addresses of each parameter in the internal program view.
	std::vector<int> params_init;
	//! A vector-array of pointers to the Identifier of each argument.
	std::vector<Identifier*> params_ptrs;
	//! Offset address of the function header in the internal program representation.
	int ptr;
	/**
	* @brief A constructor that specifies the public fields of the type.
	*/
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
	/**
	* @brief Type comparison operator.
	*
	* Compares the names of functions and the types of all their parameters..
	*
	* @param other Another function to compare.
	*/
	bool operator==(const Function& f) const {
		if (identifiers.size() != f.identifiers.size()) return false;
		bool flag = true;
		for (int i = 0; i < identifiers.size(); ++i) {
			flag &= identifiers[i].expr_type == f.identifiers[i].expr_type && identifiers[i].is_array == f.identifiers[i].is_array;
		}
		
		return name == f.name && flag;
	}
};

//! A helper class for hashing functions in standard std::unordered_map.
class FunctionHasher {
public:
	/**
	* @brief Operator () for hashing.
	*
	* Hashes the types of the function arguments and the function name.
	*
	* @param f Function for hashing.
	* 
	* @return Hash value in unsigned size_t type.
	*/
	std::size_t operator()(const Function& f) const {
		std::size_t seed = f.identifiers.size();
		for (auto& i : f.identifiers) {
			seed ^= std::hash<int>{}((int)i.expr_type  + (i.is_array << 8) + (i.array_size << 10)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return std::hash<std::string>{}(f.name) ^ (seed + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}
};