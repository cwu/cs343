#include "q2printer.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

static const int COLUMN_WIDTH = 6;

Printer::Printer(unsigned int numPhil)
    : numPhil(numPhil),states(new PrintState[numPhil]) {
    // print the header
    for (unsigned int id = 0; id < numPhil; id++) {
        stringstream str;
        str << "Phil" << id;
        flushColumn(str.str(), id);
    }

    cout << "******";
    for (unsigned int id = 1; id < numPhil; id++) {
        cout << " ******";
    }
    cout << endl;
}

Printer::~Printer() {
    delete [] states;

    // print footer
    cout << "***********************" << endl;
    cout << "Philosophers Terminated" << endl;
}

void Printer::print( unsigned int id, Philosopher::States state ) {
    if (states[id].stored || state == Philosopher::Finished) {
        flush();

        // print the finished state on its own if specified
        if (state == Philosopher::Finished) {
            for (unsigned int philId = 0; philId < numPhil; philId++) {
                flushColumn((id == philId) ? "F" : "...", philId);
            }
            return;
        }
    }

    states[id].stored = true;
    states[id].state = state;
}

void Printer::print( unsigned int id, Philosopher::States state,
                     unsigned int bite, unsigned int noodles ) {
    print(id, state);
    states[id].bite = bite;
    states[id].noodles = noodles;
}

void Printer::flush() {
    for (unsigned int id = 0; id < numPhil; id++) {
        stringstream buffer;

        if (states[id].stored) {
            buffer << (char) states[id].state;
            if (states[id].state == Philosopher::Eating) {
                buffer << states[id].bite << ',' << states[id].noodles;
            } else if (states[id].state == Philosopher::Waiting) {
                buffer << id << ',' << ((id + 1) % numPhil);
            }

            // reset the stored flag
            states[id].stored = false;
        }
        flushColumn(buffer.str(), id);
    }
}

void Printer::flushColumn(const string &col, unsigned int id) {
    cout << setw(COLUMN_WIDTH) << left << col;

    // output a space if not last, else a newline
    if (id == numPhil - 1) {
        cout << endl;
    } else {
        cout << " ";
    }
}
