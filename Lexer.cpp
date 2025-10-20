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
	std::string separators = " ,();{}";
	std::string lex;
	std::ifstream in(nameFile);
	char c;

	State state;
	while(in.get(c)) {
		if (c == ' ' or c == '\n' or c == '\t') {
			if (!lex.empty()) {
				Token token(lex, typeLexem(lex));
				tableTokens.Add(token);
				lex.clear();
			}
			continue;
		}

		if (isOperator(c) || isSeparator(c)) {
			if (!lex.empty()) {
				Token token(lex, typeLexem(lex));
				tableTokens.Add(token);
				lex.clear();
			}
			if (isOperator(c)) {
				lex += c;
				Token token(lex, TypeLexem::Operators);
				tableTokens.Add(token);
				lex.clear();
			}
			else
			{
				lex += c;
				Token token(lex, TypeLexem::SEPARATOR);
				tableTokens.Add(token);
				lex.clear();
			}
			continue;
		}



		lex += c;
	}
	
	if (!lex.empty()) {
		Token token(lex, TypeLexem::SEPARATOR);
		tableTokens.Add(token);
		lex.clear();
	}

	
}

TypeLexem Lexer::typeLexem(std::string lex)
{
	State state;
	

	if (lex.empty()) {
		return TypeLexem::UNKNOWN;
	}

	state = HandleState(lex[0]);

	for (int i = 1; i < lex.size(); i++) {
		switch (state)
		{
		case State::IN_IDENTIFIER:
			state = Ident(lex[i]);
			break;
		case State::IN_NUMBER:
			state = Number(lex[i]);
			break;
		case State::IN_OPERATOR:
			state = State::DONE;
			break;
		default:
			state = State::DONE;
			break;
		}
		if (state == State::DONE) {
			break;
		}
	}

	switch (state)
	{

	case State::IN_IDENTIFIER:
		if (lex == "int") {
			return TypeLexem::Type;
		}
		else if (isKeyWord(lex)) {
			return TypeLexem::Op;
		}
		else {
			return TypeLexem::Id;
		}
		break;

	case State::IN_NUMBER:

		return TypeLexem::Const;

		break;
	case State::IN_OPERATOR:

		return TypeLexem::Operators;

		break;
	case State::DONE:

		if (lex.size() == 1) {
			if (isOperator(lex[0])) {
				return TypeLexem::Op;
			}
			else if (isSeparator(lex[0])) {
				return TypeLexem::SEPARATOR;
			}
		}
		return TypeLexem::UNKNOWN;

		break;
	default:
		return TypeLexem::UNKNOWN;
		break;
	}

}

bool Lexer::isKeyWord(const std::string& lexem) {
	std::vector<std::string> keywords = {
		"return", "switch", "case", "break"
	};

	for (const auto& keyword : keywords) {
		if (lexem == keyword) {
			return true;
		}
	}
	return false;
}
bool Lexer::isOperator(char c)
{
	std::string operators = "=+-*/<>!";
	for (int i = 0; i < operators.size(); i++) {
		if (c == operators[i]) {
			return true;
		}
	}
	return false;
}

bool Lexer::isSeparator(char c)
{
	std::string separators = "();{},:";
	for (int i = 0; i < separators.size(); ++i) {
		if (c == separators[i]) {
			return true;
		}
	}
	return false;
}

State Lexer::Ident(char c)
{
	if (std::isalnum(c) || c == '_') {
		return State::IN_IDENTIFIER;
	}
	return State::DONE;
}

State Lexer::Number(char c)
{
	if (std::isdigit(c)) {
		return State::IN_NUMBER;
	}
	return State::DONE;
}

State Lexer::HandleState(char c)
{

	if (std::isalpha(c) || c == '_') {
		return State::IN_IDENTIFIER;
	}
	else if (std::isdigit(c)) {
		return State::IN_NUMBER;
	}
	else if (isOperator(c)) {
		return State::IN_OPERATOR;
	}
	else if (isSeparator(c)) {
		return State::DONE;
	}
	else {
		return State::DONE;
	}

}

void Lexer::print()
{
	tableTokens.Print();
}


