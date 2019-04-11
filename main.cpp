#include <iostream>
#include "src/htab.h"


int main() {
    HashTable table(10, 80);
    Flight flight(1000, 1, 0);
    Flight flight1(1000, 0, 1);

    for (int i = 1000; i <= 1020; i+=1) {
        flight1.m_number=i;
        flight.m_number=i+2;
        table.addElem(flight1);
    table.addElem(flight);

}
   //table.addElem(flight2);
    table.print();
 //   table.deleteElem(flight);
 //   table.print();
    std::cout<<std::endl<<std::endl;
    for (int i = 1000; i < 1020; i+=1) {
        flight1.m_number-=1;
        //    flight.m_number=i+2;
        //    table.addElem(flight1);
        table.deleteElem(flight1);

    }
    //table.addElem(flight2);
    table.print();

std::cout<<std::endl<<std::endl;
    for (int i = 2998; i < 3020; i+=1) {
       // flight1.m_number=i;
           flight.m_number=i+2;
            table.addElem(flight);
      //  table.deleteElem(flight1);

    }
    //table.addElem(flight2);
    table.print();


    std::cout<<std::endl<<std::endl;
    for (int i = 1000; i < 1020; i+=1) {
       // flight1.m_number-=1;
           flight.m_number=i+2;
        //    table.addElem(flight1);
        table.deleteElem(flight);

    }
    //table.addElem(flight2);
    table.print();
    return 0;
}