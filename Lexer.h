#pragma once
#include"Hash.h"
#include<fstream>
#include<vector>
#include"Syntactic analyzer.h"

enum TypeChar
{
	LETTER, DIGIT, OP, SEP, WS, OTHER, NUM_CLASSES
};

enum State {
	START, ID, NUM, OP_STATE, SEP_STATE, ERR, NUM_STATES
};




class Lexer
{
private:
	std::string nameFile;
	std::string current_lex;

	HashTable tableTokens;

	State t_Table[NUM_STATES][NUM_CLASSES] = {

		 { ID,     NUM,    OP_STATE,  SEP_STATE,  START,    ERR }, //start
		 { ID,     ID,    ERR,       ERR,        START,    ERR }, //id
		 { ERR,    NUM,   ERR,       ERR,        START,    ERR },//num
		 { ERR,    ERR,   OP_STATE,  ERR,        START,    ERR },//op
		 { ERR,    ERR,  ERR,       ERR,        START,    ERR },//sep
		 { ERR,    ERR,   ERR,       ERR,        ERR,      ERR }//err
	};

	bool isKeyWord(const std::string& lexem);
	bool isOperator(char c);
	bool isSeparator(char c);

	SyntacticAnalzer& parser;
public:
	Lexer(std::string name_file, SyntacticAnalzer& p);
	~Lexer();

	std::string readLex();
	void Analyz();	
	
	TypeChar isTypeChar(char c);

	void print();
	void print_file(std::string file_name);
	
};

