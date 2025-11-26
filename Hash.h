#pragma once
#include<string>
#include<iostream>
#include<fstream>

enum class TypeLexem
{
	IDENTIFIER, NUMBER, OPERATOR, SEPARATOR, KEYWORD, UNKNOWN, END
};

struct Token
{
	TypeLexem type;
	std::string lexema;
	Token* next;
	int line;
	int element;

	Token();
	Token(std::string lex, TypeLexem _type);

	void line_position(int line_, int col_) {
		line = line_;
		element = col_;
	}
	
};





class HashTable
{
private:
	static const int default_size = 100;
	double rehash_size = 0.75;
	Token** arr;
	int size;
	int count;

	int HashFunction(const std::string& s, int table_size);
	
public:
    HashTable();
    ~HashTable();

    void ResizeHashTable();

    void Add(Token& add_token);

	Token* Find(const std::string& lexema);

	void Print();

	void print_file(std::string file_name);

	int FindHash(Token& token);
	
	Token FindByHash(int hash);
	
};

std::string LexTypeToString(TypeLexem type);