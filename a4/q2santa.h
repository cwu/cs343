#ifndef Q2SANTA_H
#define Q2SANTA_H

#include <uC++.h>

#include "q2printer.h"
#include "q2workshop.h"

const unsigned int SANTA_ID = 0;
_Task Santa {
    Workshop &workshop;
    Printer &printer;

    void main();
  public:
    Santa( Workshop &wrk, Printer &prt );
};

#endif // Q2SANTA_H
