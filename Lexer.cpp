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
	while(in.get(c)) {
		
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

				tableTokens.Add(token);
				//Parser.push_hash_tokens(tableTokens.FindHash(token));
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


