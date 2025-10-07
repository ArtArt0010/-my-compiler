#include "Lexer.h"

Lexer::Lexer(std::string name_file)
{
	nameFile = name_file;

}

Lexer::~Lexer()
{
	nameFile.clear();
}

std::string Lexer::readLex()
{
	std::string lex;
	std::ifstream in(nameFile);
	
	in >> lex;

	return lex;
}

void Lexer::Analyz()
{
	
	std::string lex;
	std::ifstream in(nameFile);
	
	in >> lex;
	typeLexem(lex);

	
}

void Lexer::typeLexem(std::string lex)
{
	for (int i = 0; i < lex.size(); ++i) {
		if(lex[i] != )
	}


}

Token::Token(std::string lex, TypeLexem _type)
{
	lexema = lex;
	type = _type;
}
