#ifndef __A6STUDENT_H__
#define __A6STUDENT_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "office.h"

_Task Student {
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

#endif
