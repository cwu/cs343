#ifndef __A6TRUCK_H__
#define __A6TRUCK_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "config.h"

_Task Truck {
  private:
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines, maxStockPerFlavour;
    VendingMachine **machines;
    unsigned int cargo[NUM_FLAVOURS];

    void main();
  public:
    enum State {STARTING = 'S', PICK_UP_SHIPMENT = 'P', BEGIN_DELIVERY = 'd',
                FILL_UNSUCESSFUL = 'U', END_DELIVERY = 'D', FINISHED = 'F'};
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
