#pragma once
#include "additionale_struct.h"
#include <string>
#include <set>
#include<iostream>
#include<fstream>


class Semantic
{
private:
	std::string error_str = "";
	std::string postfix_str = "";
	std::set<std::string> announced;


	void check_advertisement(Node* root);

	void checkDoubleCase(Node* node);

	void postfix(Node* root);
	

public:
	Semantic();
	~Semantic();


	void analyz_start(Node* root);
	void print_postfix(Node* root);

};
