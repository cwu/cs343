#ifndef Q2REINDEER_H
#define Q2REINDEER_H

#include <uC++.h>

#include "q2printer.h"
#include "q2workshop.h"


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

#endif // Q2REINDEER_H
