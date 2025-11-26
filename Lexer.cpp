#include "Lexer.h"

Lexer::Lexer(std::string name_file, SyntacticAnalzer& p): nameFile(name_file), parser(p){}


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

	State state = START;
	bool flag_Id = true;
	
	int line = 1;
	int element = 0;
	int start_element = 0;

	while(in.get(c)) {
		if (c == '\n') {
			line++;
			element = 0;
			continue;
		}

		element++;  

		
		if (lex.empty() && !isspace(c)) {
			start_element = element;
		}

		TypeChar type_char = isTypeChar(c);
		if (type_char == TypeChar::DIGIT && state == State::ID) {
			flag_Id = false;
		}
		State next_State = t_Table[state][type_char];
		
		if (next_State == ERR or next_State == START) {
			if (!lex.empty()) {

				Token token;
				token.lexema = lex;

				switch (state)
				{
				case ID:
					if (!flag_Id) {
						token.type = TypeLexem::UNKNOWN;
						
						flag_Id = true;
					}
					else {
						if (isKeyWord(lex)) {
							token.type = TypeLexem::KEYWORD;
						}
						else {
							token.type = TypeLexem::IDENTIFIER;
						}
					}
					
					break;
				case NUM:
					if (lex[0] == '0' && lex.size()>1) {
						token.type = TypeLexem::UNKNOWN;
					}
					else {
						token.type = TypeLexem::NUMBER;
					}
					break;
				case OP_STATE:
					token.type = TypeLexem::OPERATOR;
					break;
				case SEP_STATE:
					token.type = TypeLexem::SEPARATOR;
					break;
				default:
					break;
				}

				token.line_position(line, start_element);
				tableTokens.Add(token);
				
				parser.push_hash_tokens(token);
				lex.clear();

			}
			if (isspace(c)) {
				state = START;
				continue;
			}
			state = START;
			type_char = isTypeChar(c);
			next_State = t_Table[state][type_char];
			
		}
		if (!isspace(c)) {
			lex += c;
		}

		state = next_State;
	}



	if (!lex.empty()) {
		Token tok;
		tok.lexema = lex;

		switch (state)
		{
		case ID:
			if (!flag_Id)
				tok.type = TypeLexem::UNKNOWN;
			else if (isKeyWord(lex))
				tok.type = TypeLexem::KEYWORD;
			else
				tok.type = TypeLexem::IDENTIFIER;
			break;

		case NUM:
			tok.type = TypeLexem::NUMBER;
			break;

		case OP_STATE:
			tok.type = TypeLexem::OPERATOR;
			break;

		case SEP_STATE:
			tok.type = TypeLexem::SEPARATOR;
			break;

		default:
			tok.type = TypeLexem::UNKNOWN;
		}

		tok.line_position(line, start_element);
		tableTokens.Add(tok);
		parser.push_hash_tokens(tok);
	}

	


	}


TypeChar Lexer::isTypeChar(char c)
{
	if (isalpha(c)) {
		return LETTER;
	}
	if (isdigit(c)) {
		return DIGIT;
	}
	
	if (isOperator(c)) {
		return OP;
	}
	if(isSeparator(c)) {
		return SEP;
	}
	if (isspace(c)) {
		return WS;	
	}
	return OTHER;
}


	




bool Lexer::isKeyWord(const std::string& lexem) {
	std::vector<std::string> keywords = {
		"int","return", "switch", "case", "break"
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




void Lexer::print()
{
	tableTokens.Print();
}

void Lexer::print_file(std::string file_name)
{
	tableTokens.print_file(file_name);
}


