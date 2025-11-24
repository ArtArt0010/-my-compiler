#include "Syntactic analyzer.h"

void SyntacticAnalzer::push_hash_tokens(Token t)
{
	set_of_hashes.push_back(t);
}



Node* SyntacticAnalzer::parse()
{
	pos = 0;
	Node* root = parseFunction();
	if (pos != set_of_hashes.size()) {
		std::cout << "Syntax error\n";
	}
	return root;
}

Token SyntacticAnalzer::currentToken()
{
	if (pos >= set_of_hashes.size()) {
		return Token("EOF", TypeLexem::UNKNOWN);
	}
	Token t = set_of_hashes[pos];
	return t;

}

void SyntacticAnalzer::nextToken()
{
	if (pos < set_of_hashes.size()) { pos++; }
}

bool SyntacticAnalzer::testMatch(std::string lexema)
{
	return currentToken().lexema == lexema;
}

bool SyntacticAnalzer::testMatchType(TypeLexem t)
{
	return currentToken().type == t;
}

Token SyntacticAnalzer::exists(std::string lexema)
{
	if (!testMatch(lexema)) {
		std::cout << "Expected: " + lexema + ", received: " + currentToken().lexema;
	}
	Token t = currentToken();
	nextToken();
	return t;
}

Token SyntacticAnalzer::existsType(TypeLexem t)
{
	if (!testMatchType(t))
		std::cout<<"Expected type: " + std::to_string((int)t);
	Token T = currentToken();
	nextToken();
	return T;
}





//Node* SyntacticAnalzer::parseFunction()
//{
//	Node* node = new Node("Function");
//	node->child.push_back(parseBegin());
//	node->child.push_back(parseDescriptions());
//	node->child.push_back(parseOperators());
//
//	node->child.push_back(new Node("End"));
//
//	exists("return");
//
//	Token id = existsType(TypeLexem::IDENTIFIER);
//	node->child.back()->child.push_back(new Node("Id:" + id.lexema));
//
//	exists(";");
//	exists("}");
//
//
//	return node;
//}

Node* SyntacticAnalzer::parseFunction()
{
	Node* node = new Node("Function");

	node->child.push_back(parseBegin());
	node->child.push_back(parseDescriptions());
	node->child.push_back(parseOperators());

	// end
	exists("}");

	return node;
}

Node* SyntacticAnalzer::parseBegin()
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

Node* SyntacticAnalzer::parseDescriptions()
{
	Node* node = new Node("Descriptions");

	while (testMatch("int")) {
		node->child.push_back(parseDescr());
	}

	return node;;
}

//int VarList
Node* SyntacticAnalzer::parseDescr()
{
	Node* node = new Node("Descr");

	exists("int");
	node->child.push_back(parseVarList());
	exists(";");

	return node;
}

Node* SyntacticAnalzer::parseVarList()
{
	Node* node = new Node("VarList");

	Token id = existsType(TypeLexem::IDENTIFIER);
	node->child.push_back(new Node("Id:" + id.lexema));

	while (testMatch(",")) {
		exists(",");
		Token id2 = existsType(TypeLexem::IDENTIFIER);
		node->child.push_back(new Node("Id:" + id2.lexema));
	}
	return node;
}

Node* SyntacticAnalzer::parseOperators()
{
	Node* node = new Node("Operators");

	while (
		testMatchType(TypeLexem::IDENTIFIER) or testMatch("switch")) {
		node->child.push_back(parseOp());
	}

	return node;
}
//Node* SyntacticAnalzer::parseOp()
//{
//	Node* node = new Node("Op");
//	if (testMatch("switch")) {
//		exists("switch");
//		exists("(");
//		node->child.push_back(parseExpr());
//		exists(")");       
//		exists("{");       
//		node->child.push_back(parseOptions());
//		exists("}");       
//		return node;
//	}
//	//Addign
//	Token id = existsType(TypeLexem::IDENTIFIER);
//	exists("=");
//	Node* expr = parseExpr();
//	exists(";");
//
//	Node* assign = new Node("Assign:" + id.lexema);
//	assign->child.push_back(expr);
//	node->child.push_back(assign);
//
//	return node;
//}

Node* SyntacticAnalzer::parseOp()
{
	Node* node = new Node("Op");

	
	if (testMatch("switch")) {
		exists("switch");
		exists("(");
		node->child.push_back(parseExpr());
		exists(")");      
		exists("{");   
		node->child.push_back(parseOptions());
		exists("}");      
		return node;
	}


	Token id = existsType(TypeLexem::IDENTIFIER);
	exists("=");
	Node* expr = parseExpr();
	exists(";");

	Node* assign = new Node("Assign:" + id.lexema);
	assign->child.push_back(expr);
	node->child.push_back(assign);

	return node;
}

//Node* SyntacticAnalzer::parseOptions()
//{
//	Node* node = new Node("Options");
//
//	while (testMatch("case")) {
//		exists("case");
//		Token c = existsType(TypeLexem::NUMBER);
//		exists(":");
//
//		Node* caseNode = new Node("Case:" + c.lexema);
//		caseNode->child.push_back(parseOperators());
//		exists("break");
//		exists(";");
//
//		node->child.push_back(caseNode);
//	}
//
//	return node;
//}
Node* SyntacticAnalzer::parseOptions()
{
	Node* node = new Node("Options");

	while (testMatch("case")) {
		exists("case");
		Token c = existsType(TypeLexem::NUMBER);
		exists(":");

		Node* caseNode = new Node("Case:" + c.lexema);


		while (
			testMatchType(TypeLexem::IDENTIFIER) ||
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
Node* SyntacticAnalzer::parseExpr()
{
	return parseSimpleExpr();
}

Node* SyntacticAnalzer::parseSimpleExpr()
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
Node* SyntacticAnalzer::parseTerm()
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

	throw std::runtime_error("Expected termenal, received: " + currentToken().lexema); //////////!!!!!!!!!!!!!!!!
}

void SyntacticAnalzer::printTree(Node* node, int indent)
{
	if (!node) return;


	for (int i = 0; i < indent; i++)
		std::cout << "  ";


	std::cout << node->netermenal << "\n";


	for (auto child : node->child) {
		printTree(child, indent + 1);
	}
}











SyntacticAnalzer::~SyntacticAnalzer() { }