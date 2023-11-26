#pragma once

#include <string>
#include <map>

class TrieNode {
public:
	TrieNode();
	TrieNode(char c);
	~TrieNode();
	char get_char();
	std::map<char, TrieNode*>* get_next();
	int* get_ends();
	int* get_count();
private:
	char _curr_char;
	std::map<char, TrieNode*> _next;
	int _ends;
	int _count;
};

class Trie {
public:
	Trie();
	Trie(const Trie&) = delete;
	~Trie();
	void add(std::string::iterator, std::string::iterator, TrieNode* start = nullptr);
	TrieNode* find(std::string::iterator, std::string::iterator, TrieNode* start = nullptr);
	void clear();
	int get_size();
private:
	TrieNode* _root;
};