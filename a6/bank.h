#ifndef __A6BANK_H__
#define __A6BANK_H__

#include <uC++.h>

_Monitor Bank {
  private:
    unsigned int students;
    int *accounts;
    uCondition *account_conds;

  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
