#include "Syntactic analyzer.h"

void SyntacticAnalzer::push_hash_tokens(int hahs)
{
	set_of_hashes.push_back(hahs);
}

void SyntacticAnalzer::getHashTable(HashTable& tableTokens)
{
	table = &tableTokens;
}

Node* SyntacticAnalzer::parse()
{
	pos = 0;
	Node* root = parseFunction();
	if (pos >= set_of_hashes.size()) {
		std::cout << "error\n";
	}
	return root;
}




SyntacticAnalzer::~SyntacticAnalzer() { }