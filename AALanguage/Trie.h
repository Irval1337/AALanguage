#pragma once

#include <string>
#include <map>

/**
* @brief Trie-node class.
* 
* The node stores within itself:
- node symbol
- map of transitions in children
- number of words ending in this node
- total number of words in subtrees
*/
class TrieNode {
public:
	/**
	* @brief Default constructor.
	*/
	TrieNode();
	/**
	* @brief Constructor specifying the symbol of the current node.
	* 
	* @param c Node symbol
	*/
	TrieNode(char c);
	/**
	* @brief Destructor.
	*
	* Calls all child nodes destructors, causing all subtrie nodes to be removed.
	*/
	~TrieNode();
	/**
	* @brief Gets the symbol of the current node.
	*
	* @return The symbol of the current node.
	*/
	char get_char();
	/**
	* @brief Gets transitions to child nodes.
	*
	* @return A pointer to map<char, TrieNode*> - map of transitions in children by character.
	*/
	std::map<char, TrieNode*>* get_next();
	/**
	* @brief Gets the number of strings ending in the given node.
	*
	* @return A pointer to the number of strings ending in the given node.
	*/
	int* get_ends();
	/**
	* @brief Gets the number of strings ending in subtrees.
	*
	* @return A pointer to the number of strings ending in subtrees.
	*/
	int* get_count();
private:
	char _curr_char;
	std::map<char, TrieNode*> _next;
	int _ends;
	int _count;
};

/**
* @brief String-trie class for storing reserved words.
*/
class Trie {
public:
	/**
	* @brief Default constructor.
	*/
	Trie();
	/**
	* @brief There is no copy constructor for this class.
	*/
	Trie(const Trie&) = delete;
	/**
	* @brief Destructor.
	*
	* Calls root node destructor, causing all trie nodes to be removed.
	*/
	~Trie();
	/**
	* @brief Adding a new word to the trie.
	*
	* Recursively adds a string to the tree. Asymptotics of the operation O(|S|), where |S| is the length of the input string.
	*
	* @param itBeg Iterator to the beginning of the string
	* @param itEnd Iterator to the end of the string
	* @param start Pointer to the node from which to start adding the string. If equal to nullptr, the addition will start from the root node.
	*/
	void add(std::string::iterator itBeg, std::string::iterator itEnd, TrieNode* start = nullptr);
	/**
	* @brief Searching for a string in the trie.
	*
	* Recursively finds the given string in boron. The asymptotics of the operation is O(|SS|), where |S| is the length of the input string.
	*
	* @param itBeg Iterator to the beginning of the string
	* @param itEnd Iterator to the end of the string
	* @param start Pointer to the node from which to start finding the string. If equal to nullptr, the finding will start from the root node.
	* 
	* @return A pointer to the last string node found in the tree, or nullptr if there is no such string.
	*/
	TrieNode* find(std::string::iterator itBeg, std::string::iterator itEnd, TrieNode* start = nullptr);
	/**
	* @brief Full trie clearing.
	*/
	void clear();
	/**
	* @brief Gets the number of rows added to the trie.
	* 
	* @return The number of rows in the tree.
	*/
	int get_size();
private:
	TrieNode* _root;
};