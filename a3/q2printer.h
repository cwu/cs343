#ifndef Q2PRINTER_H
#define Q2PRINTER_H

#include <uC++.h>
#include <string>
#include "q2philosopher.h"

_Monitor Printer {
    private:
        unsigned int numPhil;
        struct PrintState {
            bool stored;
            Philosopher::States state;
            unsigned int bite;
            unsigned int noodles;

            PrintState() : stored(false) {}
        };
        PrintState * states;

        void flush();
        void flushColumn(const std::string &col, unsigned int id);
    public:
        Printer( unsigned int numPhil );
        ~Printer();
        void print( unsigned int id, Philosopher::States state );
        void print( unsigned int id, Philosopher::States state,
                    unsigned int bite, unsigned int noodles );
};

#endif // Q2PRINTER_H
