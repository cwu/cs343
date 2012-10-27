#ifndef Q2WORKSHOP_H
#define Q2WORKSHOP_H

#include <uC++.h>

#include "q2printer.h"

_Monitor Workshop {
    Printer &printer;
    unsigned int elves;
    unsigned int bound;
    unsigned int deliveries;
  public:
    enum Status { Consulting, Delivery, Done };
    Workshop( Printer &prt, unsigned int E, unsigned int N, unsigned int D ); // printer, elves, bound, deliveries
    Status sleep();                               // santa calls to nap; when Santa wakes status of next action
    void deliver( unsigned int id );              // reindeer call to deliver toys
    bool consult( unsigned int id );              // elves call to consult Santa,
                                                  //   true => consultation successful, false => consultation failed
    void doneConsulting( unsigned int id );       // block Santa/elves until meeting over
    void doneDelivering( unsigned int id );       // block Santa/reindeer until all toys are delivered
    void termination( unsigned int id );          // elves call to indicate termination
};


#endif // Q2WORKSHOP_H
