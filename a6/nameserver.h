#ifndef __A6NAMESERVER_H__
#define __A6NAMESERVER_H__

#include <uC++.h>
#include "printer.h"

_Task VendingMachine;

_Task NameServer {
    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
