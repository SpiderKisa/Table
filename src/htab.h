#ifndef TABLE_HTAB_H
#define TABLE_HTAB_H


struct Flight{
    unsigned short m_number;
    unsigned short m_hours;
    unsigned short m_minutes;

public:
    Flight():m_number(0), m_hours(0), m_minutes(0){

    }
    Flight (unsigned short number, unsigned short hours, unsigned short minutes)
    : m_number(number), m_minutes(minutes), m_hours(hours){

    }
};


class HashTable{
    Flight *table;
    unsigned short *status;
    unsigned short m_loadFactor;
    unsigned int m_size;
    unsigned int initSize;
    unsigned int occupied;

    unsigned int h1 (Flight elem);

    unsigned int h2 (unsigned int collision);

public:
    HashTable(unsigned int size, unsigned short loadFactor);

    ~HashTable();

    void addElem (Flight elem);

    unsigned int searchElem (Flight elem);

    void deleteElem (Flight elem);

    void print ();
};

#endif //TABLE_HTAB_H
