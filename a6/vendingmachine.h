#ifndef __A6VENDINGMACHINE_H__
#define __A6VENDINGMACHINE_H__

#include <uC++.h>
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"
#include "config.h"

_Task VendingMachine {
  private:
    Printer &prt;
    NameServer &nameServer;
    unsigned int id, sodaCost;
    unsigned int sodaInventory[NUM_FLAVOURS];
    bool isRestocking;

    void main();

  public:
    enum State {STARTING = 'S', START_RESTOCK = 'r', END_RESTOCK = 'R',
                SODA_BOUGHT = 'B', FINISHED = 'F'};
    enum Flavours { HAM, CHEESE, EYES, PENCIL };
    enum Status { BUY, STOCK, FUNDS };		// purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    virtual ~VendingMachine();
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
