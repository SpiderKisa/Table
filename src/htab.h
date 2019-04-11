#ifndef TABLE_HTAB_H
#define TABLE_HTAB_H


struct Flight{
    unsigned short m_number;
    unsigned short m_hours;
    unsigned short m_minutes;

    Flight();

    Flight (unsigned short number, unsigned short hours, unsigned short minutes);

    void IsCorrect();
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

    unsigned int searchElem (Flight elem);

public:
    HashTable(unsigned int size, unsigned short loadFactor);

    ~HashTable();

    unsigned short addElem (Flight elem);//0 - добавлено, -1 - запись уже есть

    bool isFound (Flight elem);

    void deleteElem (Flight elem);

    void print ();
};

#endif //TABLE_HTAB_H
