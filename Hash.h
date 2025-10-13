#pragma once
#include<string>

int HashFunctionHorner(const std::string& s, int table_size, const int key);

struct HashFunction1
{
    int operator()(const std::string& s, int table_size) const;
  
};
struct HashFunction2
{
	int operator()(const std::string& s, int table_size) const;
 
};
//
//struct Token
//{
//	std::string lexema;
//	bool deletFlag;
//	Token(const std::string& value_) : lexema(value_), deletFlag(false) {}
//
//};

enum class TypeLexem
{
	Function, Begin, End, FunctionName, Descriptions, Operators, Descr, VarList, Type, Op, Options, SimpleExpr, Id, Const, int_num, id_name, SEPARATOR, UNKNOWN
};
struct Token
{
	bool deletFlag;
	TypeLexem type;
	std::string lexema;
	Token(std::string lex, TypeLexem _type);
};

class HashTable
{
private:
	static const int default_size = 8;
	double rehash_size = 0.75;
	Token** arr;
	int size;//размер массива без учёта удаленных ячеек
	int buf_size;//размер самого массива (всей таблицы)
	int full_size;


public:
    HashTable();
    ~HashTable();

    void ResizeHashTable();
    void ReHash();

    bool Find(std::string& value, const HashFunction1& hash1 = HashFunction1(), const HashFunction2& hash2 = HashFunction2());
    bool Remove(std::string& value, const HashFunction1& hash1 = HashFunction1(), const HashFunction2& hash2 = HashFunction2());
    bool Add(std::string& value, const HashFunction1& hash1 = HashFunction1(), const HashFunction2& hash2 = HashFunction2());

	int GetId(std::string& value, const HashFunction1& hash1 = HashFunction1(), const HashFunction2& hash2 = HashFunction2());
	bool ContainsId(int id) const;
	std::string GetStringById(int id) const;
};

