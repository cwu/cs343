#ifndef __A6PARENT_H__
#define __A6PARENT_H__

#include <uC++.h>
#include "printer.h"
#include "bank.h"

_Task Parent {
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#define
