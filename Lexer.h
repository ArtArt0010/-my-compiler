#pragma once
#include"Hash.h"
#include<fstream>
#include<vector>

enum TypeChar
{
	LETTER, DIGIT, UNDERSCORE, OP, SEP, WS, OTHER, NUM_CLASSES
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

		 { ID,     NUM,   ID,    OP_STATE,  SEP_STATE,  START,    ERR },
		 { ID,     ID,    ID,    ERR,       ERR,        START,    ERR },
		 { ERR,    NUM,   ERR,   ERR,       ERR,        START,    ERR },
		 { ERR,    ERR,   ERR,   OP_STATE,  ERR,        START,    ERR },
		 { ERR,    ERR,   ERR,   ERR,       ERR,        START,    ERR },
		 { ERR,    ERR,   ERR,   ERR,       ERR,        ERR,      ERR }
	};

	bool isKeyWord(const std::string& lexem);
	bool isOperator(char c);
	bool isSeparator(char c);

public:
	Lexer(std::string name_file);
	~Lexer();

	std::string readLex();
	void Analyz();
	
	TypeChar isTypeChar(char c);

	void print();
	void print_file(std::string file_name);
	
};

