#include <iostream>
#include <fstream>
#include"Lexer.h"
#include"Syntactic analyzer.h"
using namespace std;
int main(){
    ofstream out("output_parser.txt");
    SyntacticAnalzer parser;
    Lexer Lex("test.txt", parser);
    
    //Lex.Analyz();
    //Node* root = parser.parse();
    //parser.printTree(root);
    Lex.Analyz();
    Lex.print_file("output.txt");

    try {
        
        Node* root = parser.parse();
        parser.setFileName("output_parser.txt");
        parser.printTree(root);
        parser.printTreeFile(root);
        
    }
    catch (const std::runtime_error& e) {
        out << "\n"<< e.what() << std::endl;
        std::cerr << e.what() << std::endl;
        return 1; 
    }
    
   

}

