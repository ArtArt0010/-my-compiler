#pragma once
#include"Hash.h"
#include<vector>

struct Node
{
	std::string netermenal;
	std::vector<Node*> child;
	Node(const std::string& l) : netermenal(l) {}
};

class SyntacticAnalzer
{
private:
	std::vector<int> set_of_hashes;
	HashTable* table = nullptr;
	int pos = 0;
public:
	SyntacticAnalzer() = default;
//	SyntacticAnalzer(HashTable& table):_table(table){}
	~SyntacticAnalzer();


	void push_hash_tokens(int hahs);
	void getHashTable(HashTable& tableTokens);

	Node* parse();
private:
	char currentHash();
	char nextHash();

	Node* parseFunction();
	Node* parseBegin();
	Node* parseDescriptions();
	Node* parseDescr();
	Node* parseVarList();
	Node* parseOperators();
	Node* parseOp();
	Node* parseOptions();
	Node* parseExpr();
	Node* parseSimpleExpr();
	Node* parseTerm();


};

