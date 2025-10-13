#pragma once
#include"Hash.h"
#include<fstream>
#include<vector>

//enum class TypeLexem
//{
//	Function, Begin, End, FunctionName, Descriptions, Operators, Descr, VarList, Type, Op, Options, SimpleExpr, Id, Const, int_num, id_name, SEPARATOR, UNKNOWN
//};

enum class State {
	START,          // начальное состояние
	IN_IDENTIFIER,  // читаем идентификатор
	IN_NUMBER,      // читаем число
	IN_OPERATOR,    // читаем оператор
	DONE            // завершил
};

//struct Token
//{
//	TypeLexem type;
//	std::string lexema;
//	Token(std::string lex, TypeLexem _type);
//};


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
	TypeLexem typeLexem(std::string lex);

	bool isKeyWord(const std::string& lexem);
	bool isOperator(char c);
	bool isSeparator(char c);
	State Ident(char c);
	State Number(char c);
	State HandleState(char c);


	
};

