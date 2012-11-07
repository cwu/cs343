#ifndef Q2ELF_H
#define Q2ELF_H

#include <uC++.h>

#include "q2printer.h"
#include "q2workshop.h"

_Task Elf {
    unsigned int id;
    Workshop &workshop;
    Printer &printer;
    unsigned int numConsultations;

    /**
     * Executes the elf task.
     */
    void main();
  public:
    enum { CONSULTING_GROUP_SIZE = 3 };           // number of elves for a consultation with Santa
    Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations );
};

#endif // Q2ELF_H
