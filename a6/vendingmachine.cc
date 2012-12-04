#include <uC++.h>
#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
  unsigned int maxStockPerFlavour ) :
  prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), isRestocking(false)
{
  for (int i = 0; i < NUM_FLAVOURS; i++) {
    sodaInventory[i] = 0;
  }
}

VendingMachine::~VendingMachine() {}

void VendingMachine::main() {
  prt.print(Printer::Vending, id, (char)STARTING, sodaCost);

  nameServer.VMregister(this);

  while (true) {
    _Accept(~VendingMachine) {
      break;
    } or _When(!isRestocking) _Accept(buy, inventory) {
      prt.print(Printer::Vending, id, (char)START_RESTOCK);
    } or _When(isRestocking)  _Accept(restocked) {
      prt.print(Printer::Vending, id, (char)END_RESTOCK);
    }
  }

  prt.print(Printer::Vending, id, (char)FINISHED);
}

VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card ) {
  // Check stock
  if (sodaInventory[flavour] == 0) {
    return STOCK;
  }

  // Check funds
  if (sodaCost > card.getBalance()) {
    return FUNDS;
  }


  // Sucessful purchase!
  prt.print(Printer::Vending, id, (char)SODA_BOUGHT, flavour, --sodaInventory[flavour]);
  card.withdraw(sodaCost);
  return BUY;
}

unsigned int *VendingMachine::inventory() {
  isRestocking = true;
  return sodaInventory;
}

void VendingMachine::restocked() {
  isRestocking = false;
}

_Nomutex unsigned int VendingMachine::cost() {
  return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
  return id;
}
