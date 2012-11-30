#include <uC++.h>
#include "student.h"
#include "config.h"
#include "vendingmachine.h"
#include "mprng.h"

#define STARTING_WATCARD_AMOUNT 5

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
  unsigned int maxPurchases ) :
  prt(prt), nameServer(nameServer), office(cardOffice), id(id)
{
  numPurchases = rng(maxPurchases - 1) + 1;
  favouriteFlavour = rng(NUM_FLAVOURS);
  prt.print(Printer::Student, id, (char)STARTING, favouriteFlavour, numPurchases);
  watcard = office.create(id, STARTING_WATCARD_AMOUNT);
}

void Student::main() {
  while (numPurchases) {
    numPurchases--;
    yield(rng(9) + 1);

    // Find vending machine
    VendingMachine *machine = nameServer.getMachine(id);
    prt.print(Printer::Student, id, (char)SELECT_MACHINE, machine->getId());
    VendingMachine::Status st = machine->buy((VendingMachine::Flavours)favouriteFlavour, *(watcard()));

    // TODO finish
  }

  prt.print(Printer::Student, id, (char)FINISHED);
}
