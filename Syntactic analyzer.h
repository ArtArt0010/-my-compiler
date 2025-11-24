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
	std::vector<Token> set_of_hashes;

	
	int pos = 0;
public:
	SyntacticAnalzer() = default;
	~SyntacticAnalzer();


	void push_hash_tokens(Token t);
	

	Node* parse();
private:
	Token currentToken();
	void nextToken();
	bool testMatch(std::string lexema);
	bool testMatchType(TypeLexem t);
	Token exists(std::string lexema);
	Token existsType(TypeLexem t);

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
public:
	void printTree(Node* node, int indent = 0);
	void printTokens();
};

