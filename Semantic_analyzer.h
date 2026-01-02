#pragma once
#include "additionale_struct.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Hash.h"

class Semantic
{
private:
	std::string error_str = "";
	std::string postfix_str = "";
	HashTable declaration;

	void check_advertisement(Node* root);

	void checkDoubleCase(Node* node);

	void postfix(Node* root);
	

public:
	Semantic();
	~Semantic();


	void analyz_start(Node* root);
	void print_postfix(Node* root);

};
