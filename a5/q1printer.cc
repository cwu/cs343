#include "q1printer.h"

#include <iostream>

using namespace std;

Printer::Printer( unsigned int voters )
    : voters(voters), printStates(new PrintState[voters]) {
    for (unsigned int voter = 0; voter < voters ; voter++) {
        printStates[voter].hasData = false;
        cout << "Voter" << voter << '\t';
    }
    cout << endl;
    for (unsigned int voter = 0; voter < voters ; voter++) {
        cout << "======" << '\t';
    }
    cout << endl;
}

Printer::~Printer() {
    delete [] printStates;
    cout << "=================" << endl;
    cout << "All tours started" << endl;
}

void Printer::print( unsigned int id, Voter::States state ) {
    if (printStates[id].hasData) {
        flushStates();
    }

    printStates[id].hasData = true;
    printStates[id].state = state;
}

void Printer::print( unsigned int id, Voter::States state, bool vote ) {
    if (state == Voter::Finished) {
        flushStates();
        printFinished(id, vote);
        return;
    }

    if (printStates[id].hasData) {
        flushStates();
    }

    printStates[id].hasData = true;
    printStates[id].state = state;
    printStates[id].vote = vote;
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    if (printStates[id].hasData) {
        flushStates();
    }

    printStates[id].hasData = true;
    printStates[id].state = state;
    printStates[id].numBlocked = numBlocked;
}

void Printer::printFinished(unsigned int finishedVoter, bool vote) {
    for (unsigned int voter = 0; voter < voters; voter++) {
        if (voter == finishedVoter) {
            cout << "F " << vote;
        } else {
            cout << "...";
        }
        cout << '\t';
    }
    cout << endl;
}

void Printer::flushStates() {
    for (unsigned int voter = 0; voter < voters; voter++) {
        if (printStates[voter].hasData) {
            cout << ((char) printStates[voter].state);
            switch (printStates[voter].state) {
                case Voter::Vote:
                    cout << ' ' << printStates[voter].vote;
                    break;
                case Voter::Block:
                    cout << ' ' << printStates[voter].numBlocked;
                    break;
                case Voter::Unblock:
                    cout << ' ' << printStates[voter].numBlocked;
                    break;
                default:
                    break;
            }
        }
        cout << '\t';

        // reset data
        printStates[voter].hasData = false;
    }
    cout << endl;
}
