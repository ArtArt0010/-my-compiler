#pragma once
#include"Hash.h"
#include<fstream>
#include"additionale_struct.h"

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

	std::ifstream in;
	State state = START;
	bool flag_Id = true;

	int line = 1;
	int element = 0;
	int start_element = 0;

	char buffer;
	bool flag_buff = false;

	std::string separators = " ,();{}";

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


public:
	Lexer(std::string name_file);
	~Lexer();


	Token Analyz_next();	
	
	TypeChar isTypeChar(char c);

	void print();
	void print_file(std::string file_name);
	
};

