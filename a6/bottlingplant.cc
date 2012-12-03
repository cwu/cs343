#include "bottlingplant.h"
#include "mprng.h"
#include "truck.h"

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
  unsigned int timeBetweenShipments ) :
  prt(prt), nameServer(nameServer), machines(numVendingMachines),
  maxShippedPerFlavour(maxShippedPerFlavour),
  maxStockPerFlavour(maxStockPerFlavour),
  timeBetweenShipments(timeBetweenShipments),
  isClosed(false)
{
  for (int i = 0; i < NUM_FLAVOURS; i++) {
    stock[i] = 0;
  }

  // Create truck
  theTruck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
}

BottlingPlant::~BottlingPlant() {
  delete theTruck;
}

void BottlingPlant::main() {
  prt.print(Printer::BottlingPlant, (char)STARTING);

  produce();

  while (true) {
    _Accept(~BottlingPlant) {
      isClosed = true;
      exitCond.wait();
      break;
    } _Accept(getShipment) {
      // Start production run after truck picks up current run
      yield(timeBetweenShipments);
      produce();
    }
  }

  prt.print(Printer::BottlingPlant, (char)FINISHED);
}

void BottlingPlant::produce() {
  int totalProduced = 0;

  for (int i = 0; i < NUM_FLAVOURS; i++) {
    int produced = rng(maxShippedPerFlavour);
    stock[i] += produced;
    totalProduced += produced;
  }

  prt.print(Printer::BottlingPlant, (char)GENERATING, totalProduced);
}

bool BottlingPlant::getShipment( unsigned int cargo[] ) {
  if (isClosed) {
    exitCond.signal();
    return true;
  }

  prt.print(Printer::BottlingPlant, (char)PICKEDUP);
  for (int i = 0; i < NUM_FLAVOURS; i++) {
    cargo[i] = stock[i];
    stock[i] = 0;
  }

  return false;
}
