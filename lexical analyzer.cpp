#include <iostream>

#include"Lexer.h"
#include"Syntactic analyzer.h"
using namespace std;
int main()
{
    SyntacticAnalzer parser;
    Lexer Lex("test.txt", parser);
    
    Lex.Analyz();
    Lex.print();
    Lex.print_file("output.txt");
   

}

