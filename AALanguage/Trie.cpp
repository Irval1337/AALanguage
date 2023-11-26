#include "Trie.h"

TrieNode::TrieNode() {
	_curr_char = 0;
	_ends = _count = 0;
}
TrieNode::TrieNode(char c) {
	_curr_char = c;
	_ends = _count = 0;
}
TrieNode::~TrieNode() {
	for (auto& u : _next) {
		if (u.second != nullptr)
			delete u.second;
	}
}
char TrieNode::get_char() {
	return _curr_char;
}
std::map<char, TrieNode*>* TrieNode::get_next() {
	return &_next;
}
int* TrieNode::get_ends() {
	return &_ends;
}
int* TrieNode::get_count() {
	return &_count;
}

Trie::Trie() {
	_root = new TrieNode(0);
}
Trie::~Trie() {
	delete _root;
}

void Trie::add(std::string::iterator itBeg, std::string::iterator itEnd, TrieNode* start) {
	if (start == nullptr)
		start = _root;

	if (itBeg == itEnd) {
		*start->get_ends() += 1;
		return;
	}

	*start->get_count() += 1;
	char curr_char = *itBeg;

	if (start->get_next()->count(curr_char) == 0)
		start->get_next()->insert(std::make_pair(curr_char, new TrieNode(curr_char)));

	auto tmpIt = itBeg;
	tmpIt++;

	add(tmpIt, itEnd, start->get_next()->operator[](curr_char));
}
TrieNode* Trie::find(std::string::iterator itBeg, std::string::iterator itEnd, TrieNode* start) {
	if (start == nullptr)
		start = _root;

	if (itBeg == itEnd)
		return *start->get_ends() == 0 ? nullptr : start;

	char curr_char = *itBeg;
	if (start->get_next()->count(curr_char) == 0) return nullptr;

	auto tmpIt = itBeg;
	tmpIt++;
	return find(tmpIt, itEnd, start->get_next()->operator[](curr_char));
}
void Trie::clear() {
	delete _root;
	_root = new TrieNode(0);
}
int Trie::get_size() {
	return *_root->get_count();
}