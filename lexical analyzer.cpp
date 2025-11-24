#include <iostream>

#include"Lexer.h"
#include"Syntactic analyzer.h"
using namespace std;
int main()
{
    SyntacticAnalzer parser;
    Lexer Lex("test.txt", parser);
    
    Lex.Analyz();
    Node* root = parser.parse();
   // Lex.print();
   // parser.printTokens();
    parser.printTree(root);
    Lex.print_file("output.txt");
    
   

}

