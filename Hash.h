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

struct Node
{
	std::string value;
	bool deletFlag;
	Node(const std::string& value_) : value(value_), deletFlag(false) {}

};


class HashTable
{
private:
	static const int default_size = 8;
	double rehash_size = 0.75;
	Node** arr;
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

