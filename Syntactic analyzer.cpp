#include "Syntactic analyzer.h"




Node* SyntacticAnalyzer::parse()
{
	
	Node* root = parseFunction();
	if(current.type != TypeLexem::END){
		throw std::runtime_error(
			"Syntax error at line " + std::to_string(current.line) +
			", element " + std::to_string(current.element) +
			": unexpected token '" + current.lexema + "'"
			
		);
	}

	return root;
}

Token SyntacticAnalyzer::currentToken()
{
	

	if (current.type == TypeLexem::UNKNOWN) {
		throw std::runtime_error("Syntax error at line " + std::to_string(current.line) + ", col " + std::to_string(current.element) + ": unknown lexeme '" + current.lexema + "'");
		
	}

	
	return current;

}

void SyntacticAnalyzer::nextToken()
{
	current = lexer->Analyz_next();
}

bool SyntacticAnalyzer::testMatch(std::string lexema)
{
	return currentToken().lexema == lexema;
}

bool SyntacticAnalyzer::testMatchType(TypeLexem t)
{
	return currentToken().type == t;
}


Token SyntacticAnalyzer::exists( std::string lexema)
{
	Token t = currentToken();
	if (!testMatch(lexema)) {
		throw std::runtime_error(
			"Syntax error at line " + std::to_string(t.line) +
			", element " + std::to_string(t.element) +
			": expected '" + lexema +
			"'"
		);

	}
	nextToken();
	return t;
}

Token SyntacticAnalyzer::existsType(TypeLexem tType)
{
	Token t = currentToken();
	if (!testMatchType(tType)) {
		throw std::runtime_error(
			"Syntax error at line " + std::to_string(t.line) +
			", element " + std::to_string(t.element) +
			": expected type " + LexTypeToString(tType)
		);

	}
	nextToken();
	return t;
}




Node* SyntacticAnalyzer::parseFunction()
{
	Node* node = new Node("Function");

	node->child.push_back(parseBegin());
	node->child.push_back(parseDescriptions());
	node->child.push_back(parseOperators());


	Node* endNode = new Node("End");
 
	Token retToken = exists("return");
	endNode->child.push_back(new Node(retToken.lexema));

	
	Token retId = existsType(TypeLexem::IDENTIFIER);
	endNode->child.push_back(new Node("Id:" + retId.lexema));

	exists(";");
	exists("}");

	node->child.push_back(endNode);

	return node;
}


Node* SyntacticAnalyzer::parseBegin()
{
	Node* node = new Node("Begin");
	exists("int");
	Token FunctionName = existsType(TypeLexem::IDENTIFIER);
	node->child.push_back(new Node("FunctionName:" + FunctionName.lexema));

	exists("(");
	exists(")");
	exists("{");
	return node;
}

Node* SyntacticAnalyzer::parseDescriptions()
{
	Node* node = new Node("Descriptions");

	while (testMatch("int")) {
		node->child.push_back(parseDescr());
	}

	return node;;
}

//int VarList
Node* SyntacticAnalyzer::parseDescr()
{
	Node* node = new Node("Descr");

	
	Node* typeNode = new Node("Type");
	typeNode->child.push_back(new Node(currentToken().lexema)); 
	exists("int");
	node->child.push_back(typeNode);

	
	node->child.push_back(parseVarList());

	exists(";"); 

	return node;
}



Node* SyntacticAnalyzer::parseVarList()
{
	Node* node = new Node("VarList");

	while (true) {
		Token id = existsType(TypeLexem::IDENTIFIER);
		Node* idNode = new Node("Id:" + id.lexema);


		node->child.push_back(idNode);

		if (testMatch(",")) {
			exists(",");
		}
		else {
			break;
		}
	}

	return node;
}

Node* SyntacticAnalyzer::parseOperators()
{
	Node* node = new Node("Operators");

	while (
		testMatchType(TypeLexem::IDENTIFIER) or testMatch("switch")) {
		node->child.push_back(parseOp());
	}

	return node;
}


Node* SyntacticAnalyzer::parseOp()
{
	Node* node = new Node("Op");

	if (testMatch("switch")) {
		Node* switchNode = new Node("switch");
		exists("switch");

		exists("(");
		switchNode->child.push_back(parseExpr());
		exists(")");

		exists("{");
		switchNode->child.push_back(parseOptions());
		exists("}");

		node->child.push_back(switchNode);
		return node;
	}

	Token id = existsType(TypeLexem::IDENTIFIER);

	if (!testMatch("=")) {
		throw std::runtime_error(
			"Syntax error at line " + std::to_string(currentToken().line) +
			", element " + std::to_string(currentToken().element) +
			": unexpected identifier '" + id.lexema + "', expected assignment '='"
		);

	}

	Node* idNode = new Node("Id:" + id.lexema);
	Token eq = exists("=");
	Node* eqNode = new Node(eq.lexema);

	Node* expr = parseExpr();
	exists(";");

	idNode->child.push_back(eqNode);
	idNode->child.push_back(expr);
	node->child.push_back(idNode);


	return node;
}





Node* SyntacticAnalyzer::parseOptions()
{
	Node* node = new Node("Options");

	while (testMatch("case")) {
		exists("case");
		Token c = existsType(TypeLexem::NUMBER);
		exists(":");

		Node* caseNode = new Node("Case:" + c.lexema);


		while (
			testMatchType(TypeLexem::IDENTIFIER) or
			testMatch("switch")
			) {
			caseNode->child.push_back(parseOp());
		}

		exists("break");
		exists(";");
		node->child.push_back(caseNode);
	}

	return node;
}
Node* SyntacticAnalyzer::parseExpr()
{
	Node* node = parseTerm();
	while (testMatch("+") or testMatch("-")) {
		std::string op = currentToken().lexema;
		exists(op);
		Node* right = parseTerm();
		Node* op_node = new Node(op);
		Node* newNode = new Node("Expr:");

		
		newNode->child.push_back(node);
		newNode->child.push_back(op_node);
		newNode->child.push_back(right);
		node = newNode;
	}
	return node;


}


Node* SyntacticAnalyzer::parseSimpleExpr()
{
	Node* left = parseTerm();

	if (testMatch("+")) {
		exists("+");
		Node* plus = new Node("+");
		plus->child.push_back(left);
		plus->child.push_back(parseExpr());
		return plus;
	}

	return left;
}
Node* SyntacticAnalyzer::parseTerm()
{
	if (testMatchType(TypeLexem::IDENTIFIER)) {
		Token t = existsType(TypeLexem::IDENTIFIER);
		return new Node("Id:" + t.lexema);
	}
	if (testMatchType(TypeLexem::NUMBER)) {
		Token t = existsType(TypeLexem::NUMBER);
		return new Node("Const:" + t.lexema);
	}


	if (testMatch("(")) {
		exists("(");
		Node* e = parseExpr();
		exists(")");
		return e;
	}

	throw std::runtime_error("Expected termenal in line " + std::to_string(currentToken().line) + " , element " + std::to_string(currentToken().element));
	
}

void SyntacticAnalyzer::printTree(Node* node, int indent)
{
	if (!node) return;


	for (int i = 0; i < indent; i++)
		std::cout << "  ";


	std::cout << node->netermenal << "\n";


	for (auto child : node->child) {
		printTree(child, indent + 1);
	}
}






void SyntacticAnalyzer::printTreeFile(Node* node)
{
	std::ofstream out(file_output_name);
	save_in_file(out, node);
	
}



void SyntacticAnalyzer::setFileName(std::string file_name)
{
	file_output_name = file_name;
}

void SyntacticAnalyzer::save_in_file(std::ofstream& out, Node* node, int indent)
{
	if (!node) return;


	for (int i = 0; i < indent; i++)
		out << "  ";


	out << node->netermenal << "\n";


	for (auto child : node->child) {
		save_in_file(out, child, indent + 1);
	}
}







SyntacticAnalyzer::SyntacticAnalyzer(Lexer& L) : lexer(&L) {
	current = lexer->Analyz_next();
}

SyntacticAnalyzer::~SyntacticAnalyzer() { }