#include "Hash.h"

HashTable::HashTable()
{
    buf_size = default_size;
    size = 0;
    full_size = 0;
    arr = new Token * [buf_size];
    for (int i = 0; i < buf_size; ++i) {
        arr[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < buf_size; ++i) {
        if (arr[i]) {
            delete arr[i];
        }

    }
    delete[] arr;
}

void HashTable::ResizeHashTable()
{
    int last_buf_size = buf_size;

    buf_size *= 2;
    full_size = 0;
    size = 0;

    Token** tmp_arr = new Token * [buf_size];
    for (int i = 0; i < buf_size; ++i) {
        tmp_arr[i] = nullptr;
    }

    std::swap(arr, tmp_arr);

    for (int i = 0; i < last_buf_size; ++i) {
        if (tmp_arr[i] && !tmp_arr[i]->deletFlag) {
       
            Add(tmp_arr[i]->value);
        }
    }

    for (int i = 0; i < last_buf_size; ++i) {
        if (tmp_arr[i]) {
            delete tmp_arr[i];
        }
        
    }
    delete[] tmp_arr;
}

void HashTable::ReHash()
{
    full_size = 0;
    size = 0;
    Token** tmp_arr = new Token * [buf_size];

    for (int i = 0; i < buf_size; ++i) {
        tmp_arr[i] = nullptr;
    }
    std::swap(arr, tmp_arr);

    for (int i = 0; i < buf_size; ++i) {
        if (tmp_arr[i] && !tmp_arr[i]->deletFlag) {

            Add(tmp_arr[i]->value);
        }
    }

    for (int i = 0; i < buf_size; ++i) {
        if (tmp_arr[i]) {
            delete tmp_arr[i];
        }

    }
    delete[] tmp_arr;
}

bool HashTable::Find(std::string& value, const HashFunction1& hash1, const HashFunction2& hash2)
{
    int pos = hash1(value, buf_size);//позиция
    int step = hash2(value, buf_size);//шаг
    int i = 0;

    while (arr[pos] != nullptr && i < buf_size) {

        if (arr[pos]->value == value && !arr[pos]->deletFlag) {
            return true;
        }
        pos = (pos + step) % buf_size;
        ++i;

    }
    return false;
}

bool HashTable::Remove(std::string& value, const HashFunction1& hash1, const HashFunction2& hash2)
{
    int pos = hash1(value, buf_size);//позиция
    int step = hash2(value, buf_size);//шаг
    int i = 0;

    while (arr[pos] != nullptr && i < buf_size) {

        if (arr[pos]->value == value && !arr[pos]->deletFlag) {
            arr[pos]->deletFlag = true;
            --size;
            return true;
        }
        pos = (pos + step) % buf_size;
        ++i;

    }
    return false;
}

bool HashTable::Add(std::string& value, const HashFunction1& hash1, const HashFunction2& hash2)
{
    if (size + 1 > int(rehash_size * buf_size)) {
        ResizeHashTable();
    }
    else if (full_size > 2*size) {
        ReHash();//слишком много deleted элементов. нужен рехeш
    }

    int pos = hash1(value, buf_size);//позиция
    int step = hash2(value, buf_size);//шаг
    int i = 0;

    int first_delet = -1;//первый удалённый элемент

    while (arr[pos] != nullptr && i < buf_size) {

        if (arr[pos]->value == value && !arr[pos]->deletFlag) {
            return false;//проверка на существование элемента
        }

        if (arr[pos]->deletFlag && first_delet == -1) {//если в этом месте нет элемента
            first_delet = pos;
        }

        pos = (pos + step) % buf_size;
        ++i;
    }
    //если нет места создаём новый Node
    if (first_delet == -1) {
        arr[pos] = new Token(value);
        ++full_size;
    }
    else {
        arr[first_delet]->value = value;
        arr[first_delet]->deletFlag = false;
    }
    ++size;


    return true;
}
int HashTable::GetId(std::string& value, const HashFunction1& hash1, const HashFunction2& hash2)
{
    int pos = hash1(value, buf_size);
    int step = hash2(value, buf_size);
    int start_pos = pos;

    // Ищем строку в таблице
    for (int i = 0; i < buf_size; ++i) {
        if (arr[pos] == nullptr) {
            break; 
        }

        if (!arr[pos]->deletFlag && arr[pos]->value == value) {
            return pos;
        }

        pos = (pos + step) % buf_size;
        if (pos == start_pos) {
            break;
        }
    }
    return -1;
}

// Проверка существования ID
bool HashTable::ContainsId(int id) const
{
    return (id >= 0 && id < buf_size &&
        arr[id] != nullptr &&
        !arr[id]->deletFlag);
}

std::string HashTable::GetStringById(int id) const
{
    if (ContainsId(id)) {
        return arr[id]->value;
    }
    return "";
}

//bool HashTable::Add(std::string& value, const HashFunction1& hash1, const HashFunction2& hash2)
//{
//    // Проверяем необходимость рехеширования
//    if (size + 1 > int(rehash_size * buf_size)) {
//        ResizeHashTable();
//    }
//    else if (full_size > 2 * size) {
//        ReHash(); // слишком много deleted элементов
//    }
//
//    int pos = hash1(value, buf_size); // позиция
//    int step = hash2(value, buf_size); // шаг
//    int start_pos = pos; // запоминаем начальную позицию
//    int first_delet = -1; // первый удалённый элемент
//
//    // Ищем место для вставки
//    for (int i = 0; i < buf_size; ++i) {
//        if (arr[pos] == nullptr) {
//            // Нашли пустую ячейку - можно вставлять
//            break;
//        }
//
//        if (!arr[pos]->deletFlag && arr[pos]->value == value) {
//            return false; // элемент уже существует
//        }
//
//        if (arr[pos]->deletFlag && first_delet == -1) {
//            first_delet = pos; // запоминаем первую удаленную ячейку
//        }
//
//        pos = (pos + step) % buf_size;
//
//        // Если прошли полный круг
//        if (pos == start_pos) {
//            break;
//        }
//    }
//
//    // Вставляем элемент
//    if (first_delet != -1) {
//        // Используем удаленную ячейку
//        arr[first_delet]->value = value;
//        arr[first_delet]->deletFlag = false;
//        ++size;
//        return true;
//    }
//    else if (arr[pos] == nullptr) {
//        // Используем пустую ячейку
//        arr[pos] = new Node(value);
//        ++size;
//        ++full_size;
//        return true;
//    }
//
//    // Нет места (теоретически не должно происходить после рехеширования)
//    return false;
//}
int HashFunctionHorner(const std::string& s, int table_size, const int key)
{
    int hash_result = 0;
    for (int i = 0; i != s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

int HashFunction1::operator()(const std::string& s, int table_size) const
{
    return HashFunctionHorner(s, table_size, table_size - 1); // ключи должны быть взаимопросты, используем числа <размер таблицы> плюс и минус один.
}

int HashFunction2::operator()(const std::string& s, int table_size) const
{
    return HashFunctionHorner(s, table_size, table_size + 1);
}
