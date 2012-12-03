#ifndef __A6BOTTLINGPLANT_H__
#define __A6BOTTLINGPLANT_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "config.h"

_Task Truck;

_Task BottlingPlant {
  private:
    Printer &prt;
    NameServer &nameServer;
    unsigned int machines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;
    unsigned int stock [NUM_FLAVOURS];
    void produce();
    Truck *theTruck;
    int numRegisteredMachines;
    bool isClosed;

    void main();
  public:
    enum State {STARTING = 'S', GENERATING = 'G', PICKEDUP = 'P', FINISHED = 'F'};

    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    bool getShipment( unsigned int cargo[] );
};

#endif
