#include <uC++.h>
#include "vendingmachine.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
  prt(prt), numStudents(numStudents), numMachines(numVendingMachines), numRegisteredMachines(0) {
  assignments = new int[numStudents];
  machines = new VendingMachine*[numMachines];

  // Initial assignment of students to machines
  for (unsigned int i = 0; i < numStudents; i++) {
    assignments[i] = i%numMachines;
  }
}

NameServer::~NameServer() {
  delete assignments;
  delete machines;
}

void NameServer::main() {
  prt.print(Printer::NameServer, (char)STARTING);

  while (true) {
    _Accept(~NameServer)
      break;
    or _When(numRegisteredMachines == numMachines) _Accept(getMachine, getMachineList);
    or _When(numRegisteredMachines < numMachines) _Accept(VMregister);
  }

  prt.print(Printer::NameServer, (char)FINISHED);
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
  int id = vendingmachine->getId();
  prt.print(Printer::NameServer, (char)REGISTER_MACHINE, id);
  numRegisteredMachines++;
  machines[id] = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
  VendingMachine *ret = machines[assignments[id]];
  prt.print(Printer::NameServer, (char)NEW_MACHINE, id, ret->getId());
  assignments[id] = (assignments[id] + 1) % numMachines;

  return ret;
}

VendingMachine **NameServer::getMachineList() {
  return machines;
}
