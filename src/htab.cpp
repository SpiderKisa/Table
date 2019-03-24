#include <cmath>
#include <iostream>
#include "htab.h"

using namespace std;


Flight::Flight():m_number(0), m_hours(0), m_minutes(0) {}

Flight::Flight(unsigned short number, unsigned short hours, unsigned short minutes)
        : m_number(number), m_minutes(minutes), m_hours(hours){}

void Flight::IsCorrect() {
    if (m_number < 1000 || m_number > 9999){
        throw "Error: Flight's number must be in the range of 1000 to 9999.";
    }
    if (m_hours > 23){
        throw "Error: hours must be in the range of 0 to 23.";
    }
    if (m_minutes > 59){
        throw "Error: minutes must be in the range of 0 to 59.";
    }
}


HashTable::HashTable(unsigned int size, unsigned short loadFactor)
    : m_size(size), m_loadFactor(loadFactor), occupied(0), initSize(size){
    if (size == 0){
        throw "Error: the size of table must be greater than zero.";
    }
    if (loadFactor < 1 || loadFactor > 100){
        throw "Error: load factor must be in the range of 1 to 100.";
    }
    table = new Flight [size];
    status = new unsigned short [size];
    for (int i = 0; i < size; status[i] = 0, i++);
}

HashTable::~HashTable(){
    delete [] status;
    delete [] table;
}

/*
 * Ключ = часы_минуты_номер
 * Умножаем ключ на константу 0 < a < 1 и выделяем дробную часть полученного произведения
 *Умножаем полученное значение на размер таблицы и округляем в меньшую сторону
 * 0.6180339887498949
 * */

unsigned int HashTable::h1 (Flight elem){
    unsigned int key = (elem.m_hours * (unsigned int)1000000) + (elem.m_minutes * (unsigned int)10000) + elem.m_number;
    /*a - Knuth's value ~ 0.6180339887...*/
    double a = (sqrt((double)5) - 1) / 2;
    a *= key;
    double h, fracPart;
    fracPart = modf(a, &h);
    h = floor((double)m_size * fracPart);
    return (unsigned int)h;
}

/*
 * если шаг кратен размеру таблицы - зацикливание
 * нужно подобрать некратный?
 * */
unsigned int HashTable::h2 (unsigned int collision){
    unsigned int simple[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    unsigned int step = simple[0];
    for (int i = 0; (i < 10) && (m_size % simple[i] == 0); step = simple[i], i++);
    unsigned int h = collision + step;
    return h;
}

void HashTable::addElem (Flight elem){
    if (isFound(elem)){
        throw "Error: Element already exists.";
    }
    try {
        elem.IsCorrect();
        /*если количество элементов соответстует коэффициенту заполненности*/
        if (ceil((double) (occupied + 1) / m_size * 100) <= m_loadFactor) {
            unsigned int index = h1(elem);
            if (status[index] == 1) {
                do {
                    if (h2(index) > m_size - 1) { /*идем по кругу*/
                        index = h2(index) - m_size;
                    } else {
                        index = h2(index);
                    }
                } while (status[index] != 0);
            }
            status[index] = 1;
            occupied++;
            table[index] = elem;
        } else { /*extend*/
            unsigned int prevSize = m_size;
            m_size += initSize;
            occupied = 0;

            auto *prevStatus = new unsigned short[m_size];
            for (unsigned int i = 0; i < m_size; prevStatus[i] = 0, i++);
            swap(status, prevStatus);

            auto *prevTable = new Flight[m_size];
            swap(prevTable, table);

            for (unsigned int i = 0; i < prevSize; i++) {
                if (prevStatus[i] == 1) {
                    addElem(prevTable[i]);
                }
            }

            delete[] prevStatus;
            delete[] prevTable;

            addElem(elem);
        }
    } catch (const char *s){
        printf("%s\n", s);
    }
}

/*
 * возвращает адрес элемента в таблице
 * если элемент не найден - исключение
 * */
unsigned int HashTable::searchElem (Flight elem) {
    unsigned int index = h1(elem);
    int count(0);
    bool isFound = false;
    while (!isFound && (count < occupied) && (status[index] == 1)) {
        if (table[index].m_minutes == elem.m_minutes) {
            if (table[index].m_hours == elem.m_hours) {
                if (table[index].m_number == elem.m_number) {
                    isFound = true;
                }
            }
        }
        if (!isFound) {
            index = h2(index);
            count++;
        }
    }
    if (isFound) {
        return index;
    } else {
        throw "Error: record not found.";
    }
}


void HashTable::deleteElem (Flight elem){
    try {
        unsigned int index = searchElem(elem);
        unsigned int prevSize;
        if (m_size > (occupied * 100) / m_loadFactor) {
            prevSize = m_size;
            m_size -= initSize; /*narrow*/
        } else {
            prevSize = m_size;
        }

        occupied = 0;

        auto *prevStatus = new unsigned short[m_size];
        for (unsigned int i = 0; i < m_size; prevStatus[i] = 0, i++);
        swap(status, prevStatus);

        auto *prevTable = new Flight[m_size];
        swap(prevTable, table);

        for (unsigned int i = 0; i < prevSize; i++) {
            if ((prevStatus[i] == 1) && (i != index)) {
                addElem(prevTable[i]);
            }
        }

        delete[] prevStatus;
        delete[] prevTable;

    } catch(const char *s) {
        printf("%s\n", s);
    }
}

bool HashTable::isFound (Flight elem){
    try {
        searchElem(elem);
        return true;
    } catch (...){
        return false;
    }
}

void HashTable::print (){
    unsigned int count(0);
    for (int i = 0; i < m_size; i++){
        if (status[i] == 1){
            count++;
            printf("%d\t№%d\t%d:%d\n", count, table[i].m_number, table[i].m_hours, table[i].m_minutes);
        }
    }
    //printf("size: %d\n", m_size);
}

