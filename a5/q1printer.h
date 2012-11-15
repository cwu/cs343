#ifndef Q1PRINTER_H
#define Q1PRINTER_H

#include <uC++.h>

#include "q1voter.h"

_Monitor Printer { // chose one of the two kinds of type constructor
    struct PrintState {
        bool hasData;
        Voter::States state;
        bool vote;
        unsigned int numBlocked;
    };
    unsigned int voters;
    PrintState *printStates;

    /**
     * Flushes the states and prints out each state that has been filled.
     */
    void flushStates();

    /**
     * Prints the finished statement when a voter is finished.
     *
     * finishedVoter - the id of the voter that finished
     * vote - the outcome of the vote
     */
    void printFinished(unsigned int finishedVoter, bool vote);
  public:
	Printer( unsigned int voters );
    virtual ~Printer();
	void print( unsigned int id, Voter::States state );
	void print( unsigned int id, Voter::States state, bool vote );
	void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif // Q1PRINTER_H
