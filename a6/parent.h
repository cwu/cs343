#ifndef __A6PARENT_H__
#define __A6PARENT_H__

#include <uC++.h>
#include "printer.h"
#include "bank.h"

_Task Parent {
  private:
    Printer &prt;
    Bank &bank;
    unsigned int numStudents, parentalDelay;

    void main();

  public:
    enum State {STARTING = 'S', DEPOSIT = 'D', FINISHED = 'F'};
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
