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
  numPurchases = rng(1, maxPurchases);
  favouriteFlavour = rng(NUM_FLAVOURS - 1);
}

void Student::main() {
  prt.print(Printer::Student, id, (char)STARTING, favouriteFlavour, numPurchases);
  FWATCard watcard = office.create(id, STARTING_WATCARD_AMOUNT);

  for (unsigned int purchaseNum = 0; purchaseNum < numPurchases; purchaseNum++) {
    yield(rng(1, 10));

    // Find vending machine
    VendingMachine *machine = nameServer.getMachine(id);
    prt.print(Printer::Student, id, (char)SELECT_MACHINE, machine->getId());

    bool bought = false;
    while (!bought) {
        try {
            VendingMachine::Status st = machine->buy((VendingMachine::Flavours)favouriteFlavour, *(watcard()));
            switch (st) {
                case VendingMachine::STOCK:
                  // try a new machine
                  machine = nameServer.getMachine(id);
                  prt.print(Printer::Student, id, (char)SELECT_MACHINE, machine->getId());
                  break;
                case VendingMachine::FUNDS:
                  // transfer funds
                  watcard = office.transfer(id, STARTING_WATCARD_AMOUNT + machine->cost(), watcard());
                  break;
                case VendingMachine::BUY:
                  bought = true;
                  prt.print(Printer::Student, id, (char)BUY, machine->getId());
                  break;
            }
        } catch (WATCardOffice::Lost &ex) {
            watcard = office.create(id, STARTING_WATCARD_AMOUNT);
            prt.print(Printer::Student, id, (char)WATCARD_LOST, machine->getId());
        }
    }
  }

  // cleanup the watcard
  delete watcard();

  prt.print(Printer::Student, id, (char)FINISHED);
}
