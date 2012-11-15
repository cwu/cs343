#ifndef Q1VOTER_H
#define Q1VOTER_H

#include <uC++.h>

#include "q1tallyvotes.h"

class Printer;
_Task Voter {
    unsigned int id;
    TallyVotes &voteTallier;
    Printer &printer;
  public:
	enum States {
        Start = 'S',
        Vote = 'V',
        Block = 'B',
        Unblock = 'U',
        Complete = 'C',
        Finished = 'F'
    };
	Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer );
};

#endif // Q1VOTER_H
