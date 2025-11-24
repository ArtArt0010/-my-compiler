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
	Token t = currentToken();
	if (!testMatch(lexema)) {
		std::cout << "Syntax error: expected '" << lexema
			<< "', but received '" << t.lexema << "'\n";
		
	}
	nextToken();
	return t;
}

Token SyntacticAnalzer::existsType(TypeLexem tType)
{
	Token t = currentToken();
	if (!testMatchType(tType)) {
		std::cout << "Syntax error: expected type "
			<< LexTypeToString(tType)
			<< ", but received type "
			<< LexTypeToString(t.type)
			<< " ('" << t.lexema << "')\n";
		
	}
	nextToken();
	return t;
}



Node* SyntacticAnalzer::parseFunction()
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

	
	Node* typeNode = new Node("Type");
	typeNode->child.push_back(new Node(currentToken().lexema)); 
	exists("int");
	node->child.push_back(typeNode);

	
	node->child.push_back(parseVarList());

	exists(";"); 

	return node;
}



Node* SyntacticAnalzer::parseVarList()
{
	Node* node = new Node("VarList");

	while (true) {
		Token id = existsType(TypeLexem::IDENTIFIER);
		Node* idNode = new Node("Id:" + id.lexema);

		if (testMatch("=")) {
			Token eq = exists("=");            
			Node* eqNode = new Node(eq.lexema);
			Node* exprNode = parseExpr();

			idNode->child.push_back(eqNode);   
			idNode->child.push_back(exprNode); 
		}

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

Node* SyntacticAnalzer::parseOperators()
{
	Node* node = new Node("Operators");

	while (
		testMatchType(TypeLexem::IDENTIFIER) or testMatch("switch")) {
		node->child.push_back(parseOp());
	}

	return node;
}


Node* SyntacticAnalzer::parseOp()
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
	Node* idNode = new Node("Id:" + id.lexema);

	Token eq = exists("=");                  
	Node* eqNode = new Node(eq.lexema);

	Node* expr = parseExpr();
	exists(";");

	node->child.push_back(idNode);
	node->child.push_back(eqNode);
	node->child.push_back(expr);

	return node;
}





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
	Node* node = parseTerm();
	while (testMatch("+") or testMatch("-")) {
		std::string op = currentToken().lexema;
		exists(op);
		Node* right = parseTerm();
		Node* newNode = new Node(op);
		newNode->child.push_back(node);
		newNode->child.push_back(right);
		node = newNode;
	}
	return node;
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

	std::cout<<"Expected termenal, received: " + currentToken().lexema; 
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





void SyntacticAnalzer::printTokens()
{
	std::cout << "Tokens (" << set_of_hashes.size() << "):\n";
	for (size_t i = 0; i < set_of_hashes.size(); ++i) {
		std::cout << i << ": '" << set_of_hashes[i].lexema << "' type=" << LexTypeToString(set_of_hashes[i].type) << "\n";
	}
}





SyntacticAnalzer::~SyntacticAnalzer() { }