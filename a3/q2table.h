#ifndef Q2TABLE_H
#define Q2TABLE_H

#include <uC++.h>
#include <uSemaphore.h>
#include "q2printer.h"

class Table {
    // private declarations for this kind of table
    private:
        unsigned int numOfPhil;
        Printer &printer;

        uSemaphore mutex;
        uSemaphore **philWait;

        enum State {
            Thinking,
            Waiting,
            Eating
        };
        State *philStates;


        unsigned int left(unsigned int id);
        unsigned int right(unsigned int id);
    public:
        Table( unsigned int noOfPhil, Printer &prt );
        virtual ~Table();
        void pickup( unsigned int id );
        void putdown( unsigned int id );
};


#endif // Q2TABLE_H
