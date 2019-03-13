#include <cmath>
#include <iostream>
#include "htab.h"

using namespace std;

HashTable::HashTable(unsigned int size, unsigned short loadFactor)
    : m_size(size), m_loadFactor(loadFactor), occupied(0), initSize(size){
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
    //unsigned int key = (((elem.m_hours << 2) + elem.m_minuts) << 4) + elem.m_number;
    unsigned int key = (elem.m_hours << 6) + (elem.m_minuts << 4) + elem.m_number;

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
unsigned int HashTable::h2 (int collision){
    unsigned int simple[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    unsigned int step = simple[0];
    for (int i = 0; (i < 10) && (m_size % simple[i] ==0); step = simple[i], i++);
    unsigned int h = collision + step;
    return h;
}

void HashTable::extendTable (){
    unsigned int prevSize = m_size;
    m_size += initSize;
    occupied = 0;

    unsigned short *prevStatus = new unsigned short [m_size];
    for (unsigned int i = 0; i < m_size; prevStatus[i] = 0, i++);
    swap (status, prevStatus);

    Flight *prevTable = new Flight [m_size];
    swap(prevTable, table);

    for (unsigned int i = 0; i < prevSize; i++){
        if (prevStatus[i] == 1){
            addElem(prevTable[i]);
        }
    }
    delete [] prevStatus;
    delete [] prevTable;
}

void HashTable::addElem (Flight elem){
    if (ceil((double)occupied / m_size * 100) <= m_loadFactor) {
        int index = h1(elem);
        if (status[index] == 1) {
            do {
                if (h2(index) > m_size - 1) { /*идем по кругу*/
                    int x = m_size - 1 - index;
                    index = index - 1 - x;
                } else {
                    index = h2(index);
                }
            } while (status[index] == 0);
        }
        status[index] = 1;
        occupied++;
        table[index] = elem;
    } else {
        extendTable();
        addElem(elem);
    }
}


/*
 * returns adress of elem
 * if there is no this elem in table, throws an exception
 * */
unsigned int HashTable::searchElem (Flight elem){
    //int key = ((((elem.m_hours) << 2) + elem.m_minuts) << 2) + elem.m_number;
    unsigned int index = h1(elem);
    int count(1);
    bool isFound = false;
    while (!isFound && count < occupied){
        if (table[index].m_minuts == elem.m_minuts) {
            if (table[index].m_hours == elem.m_hours) {
                if (table[index].m_number == elem.m_number) {
                    isFound = true;
                }
            }
        }
        if (!isFound){
            index = h2(index);
            count++;
        }
    }
    if (isFound){
        return index;
    } else throw "Error: record not found.";
}

void HashTable::deleteElem (Flight elem){
    try {
        unsigned int index = searchElem(elem);
        unsigned int key = ((((elem.m_hours) << 2) + elem.m_minuts) << 2) + elem.m_number;
        if (index == h1(elem)){
            occupied--;
            status[index] = 0;
        } else {
            unsigned int prevSize = m_size;
            m_size -= 1;
            occupied = 0;

            unsigned short *prevStatus = new unsigned short [m_size];
            for (unsigned int i = 0; i < m_size; prevStatus[i] = 0, i++);
            swap (status, prevStatus);

            Flight *prevTable = new Flight [m_size];
            swap(prevTable, table);

            for (unsigned int i = 0; i < prevSize; i++){
                if ((prevStatus[i] == 1) && (i != index)){
                    addElem(prevTable[i]);
                }
            }
            delete [] prevStatus;
            delete [] prevTable;
        }
    } catch(const char *s) {
        printf("%s\n", s);
    }

}

void HashTable::print (){
    for (int i = 0; i < m_size; i++){
        if (status[i] == 1){
            printf("№ %d\t%d:%d\n", table[i].m_number, table[i].m_hours, table[i].m_minuts);
        }
    }
}

