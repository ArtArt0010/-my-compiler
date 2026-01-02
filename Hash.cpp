#include "Hash.h"

int HashTable::HashFunction(const std::string& s, int table_size)
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

    arr = new Token_node* [size];
    for (int i = 0; i < size; ++i) {
        arr[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < size; ++i) {
        Token_node* current = arr[i];
        while (current != nullptr) {
            Token_node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] arr;
}

void HashTable::ResizeHashTable()
{
    int new_size = size*2;



    Token_node** newArr = new Token_node * [new_size];

    for (int i = 0; i < new_size; ++i) {
        newArr[i] = nullptr;
    }

   
    for (int i = 0; i < size; ++i) {
        Token_node* tmp = arr[i];
        while (tmp != nullptr) {
            Token_node* next = tmp->next;
            int id = HashFunction(tmp->token.lexema, new_size);
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
  
    int id = HashFunction(add_token.lexema, size);
    Token_node* tmp = arr[id];
    while (tmp != nullptr) {
        if (tmp->token.lexema == add_token.lexema) {
            return;
        }
        tmp = tmp->next;
    }

    Token_node* n = new Token_node;
    n->token = add_token;
    n->next = arr[id];
    arr[id] = n;
    count++;
   
}

void HashTable::AddStr(std::string s)
{
    if (count + 1 > static_cast<int>(rehash_size * size)) {
        ResizeHashTable();
    }

    int id = HashFunction(s, size);
    Token_node* tmp = arr[id];
    while (tmp != nullptr) {
        if (tmp->token.lexema == s) {
            return;
        }
        tmp = tmp->next;
    }

    Token_node* n = new Token_node;
    n->key = s;
    n->next = arr[id];
    arr[id] = n;
    count++;
}

bool HashTable::Exist_check(std::string s)
{
    int id = HashFunction(s, size);

    Token_node* tmp = arr[id];

    while (tmp) {
        if (tmp->key == s) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

Token* HashTable::Find(const std::string& lexema) {
    int id = HashFunction(lexema, size);
    Token_node* tmp = arr[id];
    while (tmp) {
        if (tmp->token.lexema == lexema)
            return &tmp->token;
        tmp = tmp->next;
    }
    return nullptr;
}

void HashTable::Print() {
    std::cout << "\n--- Hash Table ---\n";
    for (int i = 0; i < size; ++i) {
        if (arr[i]) {
            std::cout << "[" << i << "]: ";
            Token_node* tmp = arr[i];
            while (tmp) {
                std::cout <<"type lexem: "<< LexTypeToString(tmp->token.type)<<" | " << tmp->token.lexema << " -> ";
                tmp = tmp->next;
            }
            std::cout << "NULL\n";
        }
    }
    std::cout << "------------------\n";
}

void HashTable::print_file(std::string file_name)
{
    std::ofstream out(file_name);
    for (int i = 0; i < size; ++i) {
        if (arr[i]) {
            
            Token_node* tmp = arr[i];
            while (tmp) {
                out << LexTypeToString(tmp->token.type) << " | " << tmp->token.lexema << " | "<< "[" << i << "]"<<"\n";
                tmp = tmp->next;
            }
           
        }
    }
}

int HashTable::FindHash(Token& token)
{
    int id = HashFunction(token.lexema, size);
  
    return id;
}

Token HashTable::FindByHash(int hash)
{
    Token_node* tmp = arr[hash];
    
    return tmp->token;
}

std::string LexTypeToString(TypeLexem type)
{
    switch (type)
    {
    case TypeLexem::IDENTIFIER:        return "ID";
    case TypeLexem::KEYWORD:     return "KeyWords";
    case TypeLexem::NUMBER:     return "Const";
    case TypeLexem::OPERATOR:          return "Operator";
    case TypeLexem::SEPARATOR:   return "Separator";
    case TypeLexem::UNKNOWN:     return "Unknown";
    default:                     return "???";
    }
}


