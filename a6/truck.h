#ifndef __A6TRUCK_H__
#define __A6TRUCK_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"

_Task Truck {
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
