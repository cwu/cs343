#ifndef Q2PHILOSOPHER_H
#define Q2PHILOSOPHER_H

#include <uC++.h>

class Table;
class Printer;
_Task Philosopher {
    private:
        unsigned int id;
        unsigned int noodles;
        Table &table;
        Printer &printer;

        void main();
    public:
        enum States {
            Thinking = 'T',
            Hungry = 'H',
            Eating = 'E',
            Waiting = 'W',
            Finished = 'F'
        };
        Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt );
};


#endif // Q2PHILOSOPHER_H
