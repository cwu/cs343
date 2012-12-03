#include <uC++.h>
#include "truck.h"
#include "vendingmachine.h"
#include "mprng.h"

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
  unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
  prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
  maxStockPerFlavour(maxStockPerFlavour)
{
  for (int i = 0; i < NUM_FLAVOURS; i++) {
    cargo[i] = 0;
  }
}

void Truck::main() {
  prt.print(Printer::Truck, (char)STARTING);
  machines = nameServer.getMachineList();

  while(true) {
    // Get coffee from timmies
    yield(rng(1, 10));

    // Get bottles produced from plant
    bool quit = plant.getShipment(cargo);
    if (quit) break; // plant closing down, time to exit

    int numBottles = 0;
    for (int i = 0; i < NUM_FLAVOURS; i++) {
      numBottles = cargo[i];
    }
    prt.print(Printer::Truck, (char)PICK_UP_SHIPMENT, numBottles);

    // Deliver to machines
    for (unsigned int i = 0; i < numVendingMachines; i++) {
      int machine_id = machines[i]->getId();
      prt.print(Printer::Truck, (char)BEGIN_DELIVERY, machine_id, numBottles);
      unsigned int *inventory = machines[i]->inventory();

      // Stock each flavour up to maxStockPerFlavour and update counts
      int unstocked_sum = 0;
      for (unsigned int f = 0; f < NUM_FLAVOURS; f++) {
        int transfer = maxStockPerFlavour - inventory[i];
        if (transfer > cargo[i]) {
          transfer = cargo[i];
          unstocked_sum += transfer - cargo[i];
        }
        numBottles -= transfer;
        inventory[i] += transfer;
        cargo[i] -= transfer;
      }

      if (unstocked_sum > 0) {
        prt.print(Printer::Truck, (char)FILL_UNSUCESSFUL, machine_id, unstocked_sum);
      }

      machines[i]->restocked();
      prt.print(Printer::Truck, (char)END_DELIVERY, machine_id, numBottles);

      // end delivery when no more in cargo
      if (numBottles == 0) break;
    }
  }

  prt.print(Printer::Truck, (char)FINISHED);
}
