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

int HashTable::h1 (Flight elem){
    const int key = ((((elem.m_hours) << 2) + elem.m_minuts) << 2) + elem.m_number;
    /*a - Knuth's value ~ 0.6180339887...*/
    double a = (sqrt((double)5) - 1) / 2;
    a *= key;
    double h, fracPart;
    fracPart = modf(a, &h);
    h = floor((double)m_size * fracPart);
    return (int)h;
}

/*
 * если шаг кратен размеру таблицы - зацикливание
 * нужно подобрать некратный?
 * */
int HashTable::h2 (int collision){
    int simple[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    int step = simple[0];
    for (int i = 0; (i < 10) && (m_size % simple[i] ==0); step = simple[i], i++);
    int h = collision + step;
    return h;
}

void HashTable::extendTable (){
    int prevSize = m_size;
    m_size += initSize;
    occupied = 0;

    unsigned short *prevStatus = new unsigned short [m_size];
    for (int i = 0; i < m_size; prevStatus[i] = 0, i++);
    swap (status, prevStatus);

    Flight *prevTable = new Flight [m_size];
    swap(prevTable, table);

    for (int i = 0; i < prevSize; i++){
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

void HashTable::deleteElem (Flight elem){

}

int HashTable::searchElem (Flight elem){

}

void HashTable::print (){
    for (int i = 0; i < m_size; i++){
        if (status[i] == 1){
            printf("№ %d\t%d:%d\n", table[i].m_number, table[i].m_hours, table[i].m_minuts);
        }
    }
}
