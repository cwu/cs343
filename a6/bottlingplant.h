#ifndef __A6BOTTLINGPLANT_H__
#define __A6BOTTLINGPLANT_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"

_Task BottlingPlant {
    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    bool getShipment( unsigned int cargo[] );
};

#endif
