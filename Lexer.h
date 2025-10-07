#pragma once
#include"Hash.h"
#include<fstream>

enum class TypeLexem
{
	Function, Begin, End, FunctionName, Descriptions, Operators, Descr, VarList, Type, Op, Options, SimpleExpr, Id, Const
};

struct Token
{
	TypeLexem type;
	std::string lexema;
	Token(std::string lex, TypeLexem _type);
};


class Lexer
{
private:
	std::string nameFile;
	std::string current_lex;

	HashTable tableTokens;

	HashFunction1 hash1;
	HashFunction2 hash2;


public:
	Lexer(std::string name_file);
	~Lexer();

	std::string readLex();
	void Analyz();
	void typeLexem(std::string lex);

};

