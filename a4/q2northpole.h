#ifndef Q2NORTHPOLE_H
#define Q2NORTHPOLE_H

#include <uC++.h>

#include "q2printer.h"

_Monitor Workshop {
    // private members go here
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

const unsigned int SANTA_ID = 0;
_Task Santa {
    Workshop &workshop;
    Printer &printer;

    void main();
  public:
    Santa( Workshop &wrk, Printer &prt );
};

_Task Elf {
    unsigned int id;
    Workshop &workshop;
    Printer &printer;
    unsigned int numConsultations;

    void main();
  public:
    enum { CONSULTING_GROUP_SIZE = 3 };           // number of elves for a consultation with Santa
    Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations );
};

_Task Reindeer {
    unsigned int id;
    Workshop &workshop;
    Printer &printer;
    unsigned int numDeliveries;

    void main();
  public:
    enum { MAX_NUM_REINDEER = 5 };                // number of reindeer in system for delivering toys
    Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries );
};

#endif // Q2NORTHPOLE_H
