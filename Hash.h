#pragma once
#include<string>
#include<iostream>

enum class TypeLexem
{
	Function, Begin, End, FunctionName, Descriptions, Operators, Descr, VarList, Type, Op, Options, SimpleExpr, Id, Const, int_num, id_name, SEPARATOR, UNKNOWN
};

struct Token
{
	TypeLexem type;
	std::string lexema;
	Token* next;
	Token(std::string lex, TypeLexem _type);
	
};





class HashTable
{
private:
	static const int default_size = 100;
	double rehash_size = 0.75;
	Token** arr;
	int size;
	int count;

	int HashFunctionHorner(const std::string& s, int table_size);
public:
    HashTable();
    ~HashTable();

    void ResizeHashTable();

    void Add(Token& add_token);

	Token* Find(const std::string& lexema);

	void Print();


};

std::string LexTypeToString(TypeLexem type);