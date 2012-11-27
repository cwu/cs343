#ifndef __A6BANK_H__
#define __A6BANK_H__

#include <uC++.h>

_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
