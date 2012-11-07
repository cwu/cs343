#include "q2printer.h"
#include "q2reindeer.h"

#include <iostream>
using namespace std;

Printer::Printer( const unsigned int MAX_NUM_ELVES )
    : numElves(MAX_NUM_ELVES), numIds(1 + numElves + Reindeer::MAX_NUM_REINDEER),
      printStates(new PrintState[numIds]) {
    for (unsigned int id = 0; id < numIds; id++) {
        printStates[id].hasData = false;
        printStates[id].numBlocked = 0;
    }

    cout << "Sa\t";
    for (unsigned int e = 1; e <= numElves; e++) {
        cout << 'E' << e << '\t';
    }
    for (unsigned int r = 1; r <= Reindeer::MAX_NUM_REINDEER; r++) {
        cout << 'R' << r << '\t';
    }
    cout << endl;

    for (unsigned int i = 0; i < numIds; i++) {
        cout << "--\t";
    }
    cout << endl;
}

Printer::~Printer() {
    delete [] printStates;
    cout << "Workshop closed" << endl;
}

void Printer::print( unsigned int id, States state ) {
    // automatically print the finished state if we receive it
    if (state == Finished) {
        flushStates();
        printFinished(id);
        return;
    }

    // if we already stored data for this id then print the states
    if (printStates[id].hasData) {
        flushStates();
    }

    // store the state
    printStates[id].hasData = true;
    printStates[id].state = state;
}

void Printer::print( unsigned int id, States state, unsigned int numBlocked ) {
    print(id, state);

    // store the extra state
    printStates[id].numBlocked = numBlocked;
}

void Printer::printFinished(unsigned int finishedId) {
    for (unsigned int id = 0; id < numIds; id++) {
        if (id == finishedId) {
            cout << 'F';
        } else {
            cout << "...";
        }
        cout << '\t';
    }
    cout << endl;
}

void Printer::flushStates() {
    for (unsigned int id = 0; id < numIds; id++) {
        if (printStates[id].hasData) {
            cout << ((char) printStates[id].state);
            if (printStates[id].numBlocked > 0) {
                cout << ' ' << printStates[id].numBlocked;
            }
        }
        cout << '\t';

        // reset data
        printStates[id].hasData = false;
        printStates[id].numBlocked = 0;
    }
    cout << endl;
}
