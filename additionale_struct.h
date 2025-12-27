#pragma once
#include <vector>
#include<string>

enum class TypeLexem
{
	IDENTIFIER, NUMBER, OPERATOR, SEPARATOR, KEYWORD, UNKNOWN, END
};

struct Node
{
	std::string netermenal;
	std::vector<Node*> child;
	Node(const std::string& l) : netermenal(l) {}
};

struct  Token
{
	TypeLexem type;
	std::string lexema;
	Token* next;
	int line;
	int element;

	Token(){
		lexema = "";
		type = TypeLexem::UNKNOWN;
	}
	Token(std::string lex, TypeLexem _type) {
		lexema = lex;
		type = _type;
	}

	void line_position(int line_, int col_) {
		line = line_;
		element = col_;
	}
};