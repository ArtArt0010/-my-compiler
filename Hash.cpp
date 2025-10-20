#include "Hash.h"

int HashTable::HashFunctionHorner(const std::string& s, int table_size)
{
    const int key = 31;
    int hash_result = 0;
    for (int i = 0; i != s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

HashTable::HashTable()
{
    size = default_size;
    count = 0;

    arr = new Token * [size];
    for (int i = 0; i < size; ++i) {
        arr[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < size; ++i) {
        Token* current = arr[i];
        while (current != nullptr) {
            Token* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] arr;
}

void HashTable::ResizeHashTable()
{
    int new_size = size*2;



    Token** newArr = new Token* [new_size];

    for (int i = 0; i < new_size; ++i) {
        newArr[i] = nullptr;
    }

   
    for (int i = 0; i < size; ++i) {
        Token* tmp = arr[i];
        while (tmp != nullptr) {
            Token* next = tmp->next;
            int id = HashFunctionHorner(tmp->lexema, new_size);
            tmp->next = newArr[id];
            newArr[id] = tmp;
            tmp = next;
        }
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = newArr;
    size = new_size;
}





void HashTable::Add(Token& add_token)
{
 
    if (count + 1 > static_cast<int>(rehash_size * size)) {
        ResizeHashTable();
    }
  
    int id = HashFunctionHorner(add_token.lexema, size);
    Token* tmp = arr[id];
    while (tmp != nullptr) {
        if (tmp->lexema == add_token.lexema) {
            return;
        }
        tmp = tmp->next;
    }

    
    Token* new_token = new Token(add_token);
    new_token->next = arr[id];
    arr[id] = new_token;
    count++;
   
}
Token* HashTable::Find(const std::string& lexema) {
    int id = HashFunctionHorner(lexema, size);
    Token* tmp = arr[id];
    while (tmp) {
        if (tmp->lexema == lexema)
            return tmp;
        tmp = tmp->next;
    }
    return nullptr;
}

void HashTable::Print() {
    std::cout << "\n--- Hash Table ---\n";
    for (int i = 0; i < size; ++i) {
        if (arr[i]) {
            std::cout << "[" << i << "]: ";
            Token* tmp = arr[i];
            while (tmp) {
                std::cout <<"type lexem: "<< LexTypeToString(tmp->type)<<" | " << tmp->lexema << " -> ";
                tmp = tmp->next;
            }
            std::cout << "NULL\n";
        }
    }
    std::cout << "------------------\n";
}

std::string LexTypeToString(TypeLexem type)
{
    switch (type)
    {
    case TypeLexem::Type:        return "Type";
    case TypeLexem::Id:     return "Id";
    case TypeLexem::Const:     return "Const";
    case TypeLexem::Op:          return "Op";
    case TypeLexem::SEPARATOR:   return "Separator";
    case TypeLexem::UNKNOWN:     return "Unknown";
    case TypeLexem::Operators: return "Optrator";
    default:                     return "???";
    }
}

Token::Token(std::string lex, TypeLexem _type)
{
    lexema = lex;
    type = _type;
}
