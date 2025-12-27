#pragma once
#include"Hash.h"
#include<vector>
#include <fstream>
#include "additionale_struct.h"
#include "Lexer.h"

class SyntacticAnalyzer
{
private:
	std::string file_output_name;
	Lexer* lexer;
	Token current;
	int pos = 0;
public:
	SyntacticAnalyzer(Lexer& L);
	~SyntacticAnalyzer();


	
	

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
	void printTreeFile(Node* node);
	
	
	void setFileName(std::string file_name);
private:
	void save_in_file(std::ofstream& out, Node* node, int indent = 0);
	
};

