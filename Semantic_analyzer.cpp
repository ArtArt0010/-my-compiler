#include "Semantic_analyzer.h"

void Semantic::check_advertisement(Node* root)
{
	
	if (root->netermenal == "Descr") {
		for (int i = 0; i < root->child.size(); i++) {
			if (root->child[i]->netermenal == "VarList") {
				Node* tmp = root->child[i];

				for (int j = 0; j < tmp->child.size(); j++) {
					if (tmp->child[j]->netermenal.find("Id:") != std::string::npos) {
						std::string n = tmp->child[j]->netermenal.substr(3);
						if (announced.find(n) != announced.end()) {
							std::cout << "Error double!"<<n<<"\n";
						}
						else {
							announced.insert(n);
						}
					}
				}
			}
		}
	

	}

	if (root->netermenal.find("Id:") != std::string::npos) {
		std::string n = root->netermenal.substr(3);
		if (announced.find(n) == announced.end()) {
			std::cout << "Error not announced! " << n << "\n";
		}
	}
	if (root->netermenal == "switch") {
		checkDoubleCase(root);
	}



	for (int i = 0; i < root->child.size(); i++) {
		check_advertisement(root->child[i]);
	}
}



void Semantic::checkDoubleCase(Node* node)
{	
	Node* opt = nullptr;
	for (int i = 0; i < node->child.size(); i++) {
		if (node->child[i]->netermenal == "Options") {
			opt = node->child[i];
			break;
		}
	}
	if (opt == nullptr) {
		return;
	}

	std::set<std::string> set_case;

	for (int i = 0; i < opt->child.size(); i++) {
		if (opt->child[i]->netermenal.find("Case:") != std::string::npos) {
			std::string n = opt->child[i]->netermenal.substr(5);
			if (set_case.find(n) != set_case.end()) {
				std::cout << "Error: duplicate case " << n << "\n";
			}
			else {
				set_case.insert(n);
			}
		}
	}
	
}

Semantic::Semantic()
{
}

Semantic::~Semantic()
{
}

void Semantic::analyz_start(Node* root)
{
	check_advertisement(root);
}

void Semantic::print_postfix(Node* root)
{
	postfix(root);
	std::cout <<"\n\n\n"<< postfix_str;
}

void Semantic::postfix(Node* root)
{
	if (root->netermenal == "int") {
		postfix_str += "int ";
	}
	if (root->netermenal == "VarList") {
		for (int i = 0; i < root->child.size(); i++) {
			postfix_str += root->child[i]->netermenal.substr(3) + " ";
		}
		postfix_str +=  "DECL\n";
	}
	std::stack<std::string > n;
	std::stack<std::string > op;
	std::string buf;
	if (root->netermenal == "Id:") {
		for (int i = 0; i < root->child.size(); i++) {
			Node* tmp = root->child[i];
			if (tmp->netermenal == "=") {
				n.push(root->netermenal.substr(3));
				op.push("=");
				
			}
		}
	}
	
	for (int i = 0; i < root->child.size(); i++) {
		postfix(root->child[i]);
	}
}






