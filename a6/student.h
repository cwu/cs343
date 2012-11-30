#ifndef __A6STUDENT_H__
#define __A6STUDENT_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "office.h"
#include "watcard.h"

_Task Student {
  private:
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &office;
    FWATCard watcard;
    unsigned int id, numPurchases, favouriteFlavour;

    void main();

  public:
    enum State {STARTING = 'S', SELECT_MACHINE = 'V', BUY = 'B', WATCARD_LOST = 'L', FINISHED = 'F'};

    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

#endif
