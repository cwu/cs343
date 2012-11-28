#ifndef __A6NAMESERVER_H__
#define __A6NAMESERVER_H__

#include <uC++.h>
#include "printer.h"

_Task VendingMachine;

_Task NameServer {
  private:
    Printer &prt;
    unsigned int numStudents, numMachines, numRegisteredMachines;
    int *assignments;
    VendingMachine **machines;

    void main();

  public:
    enum State {STARTING = 'S', REGISTER_MACHINE= 'R', NEW_MACHINE = 'N', FINISHED = 'F'};

    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
