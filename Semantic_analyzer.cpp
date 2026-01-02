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

						if (declaration.Exist_check(n)) {
							std::cout << "Error double!"<<n<<"\n";
							error_str += "Error double! " + n + "\n";
						}
						else {
							declaration.AddStr(n);
						}
					}
				}
			}
		}
	

	}

	if (root->netermenal.find("Id:") != std::string::npos) {
		std::string n = root->netermenal.substr(3);
		if (!declaration.Exist_check(n)) {
			std::cout << "Error not announced! " << n << "\n";
			error_str += "Error not announced! " + n + "\n";
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

	HashTable uniq_cases;

	for (int i = 0; i < opt->child.size(); i++) {
		if (opt->child[i]->netermenal.find("Case:") != std::string::npos) {
			std::string n = opt->child[i]->netermenal.substr(5);
			if (uniq_cases.Exist_check(n)) {
				std::cout << "Error: duplicate case " << n << "\n";
				error_str += "Error: duplicate case " + n + "\n";
			}
			else {
				uniq_cases.AddStr(n);
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
	std::ofstream out("output_semantic.txt");
	postfix(root);

	std::cout <<"\n\n\n"<< postfix_str.substr(0 , postfix_str.size()-2);
	out << error_str;
	out<< "\n\n\n" << postfix_str.substr(0, postfix_str.size() - 2);
}



void Semantic::postfix(Node* root)
{
	if (root == nullptr) return;

	

	if (root->netermenal == "Op") {

		if (root->child[0]->netermenal == "switch") {
			Node* sw = root->child[0];
			Node* opt = sw->child[1];

			for (int i = 0; i < opt->child.size(); i++) {
				Node* c = opt->child[i];
				if (i > 0) {
					postfix_str += "m" + std::to_string(i) + " DEFL ";
				}

				postfix(sw->child[0]); 
				postfix_str += c->netermenal.substr(5) + " == ";

				postfix_str += "m" + std::to_string(i + 1) + " BF ";
				

				for (int j = 0; j < c->child.size(); j++) {
					postfix(c->child[j]);
				}
				postfix_str.pop_back();
				postfix_str += "m" + std::to_string(opt->child.size()) + " BRL ";

				
				postfix_str += "\n";
			}
			postfix_str += "m" + std::to_string(opt->child.size()) + " DEFL \n";

			return;
		}

		Node* id = root->child[0];
		
		postfix_str += id->netermenal.substr(3) + " ";
		postfix(id->child[1]);
		postfix_str += "= \n";

		return;
	}




	if (root->netermenal == "Expr:") {
		postfix(root->child[0]);          
		postfix(root->child[2]);     
		postfix_str += root->child[1]->netermenal + " ";


		return;
	}


	if (root->netermenal.find("Id:") != std::string::npos) {
		postfix_str += root->netermenal.substr(3) + " ";
		return;
	}


	if (root->netermenal.find("Const:") != std::string::npos) {
		postfix_str += root->netermenal.substr(6) + " ";
		return;
	}


	


	if (root->netermenal == "VarList") {
		postfix_str += "int ";
		for (int i = 0; i < root->child.size(); i++) {
			postfix_str += root->child[i]->netermenal.substr(3) + " ";
		}
		postfix_str += std::to_string(root->child.size()+1) + " DECL\n";
		return;
	}

	if (root->netermenal == "End") {

	}

	for (int i = 0; i < root->child.size(); i++) {
		postfix(root->child[i]);
	}
}








