#ifndef TABLE_HTAB_H
#define TABLE_HTAB_H


struct Flight{
    unsigned short m_number;
    unsigned short m_hours;
    unsigned short m_minuts;
};


class HashTable{
    Flight *table;
    unsigned short *status;
    unsigned short m_loadFactor;
    unsigned int m_size;
    unsigned int initSize;
    unsigned int occupied;

    HashTable(unsigned int size, unsigned short loadFactor);

    ~HashTable();

    int h1 (Flight elem);

    /*collision - место коллизии*/
    int h2 (int collision);

    void extendTable ();

public:
    void addElem (Flight elem);

    void deleteElem (Flight elem);

    int searchElem (Flight elem);

    void print ();
};


#endif //TABLE_HTAB_H
