#pragma once
#include <vector>
#include<string>



struct Node
{
	std::string netermenal;
	std::vector<Node*> child;
	Node(const std::string& l) : netermenal(l) {}
};