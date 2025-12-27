#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include"additionale_struct.h"



struct Token_node
{
	Token token;
	Token_node* next = nullptr;
	
	
};





class HashTable
{
private:
	static const int default_size = 100;
	double rehash_size = 0.75;
	Token_node** arr;
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