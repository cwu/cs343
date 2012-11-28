#include <uC++.h>
#include "office.h"

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
  prt(prt), bank(bank), numCouriers(numCouriers)
{
}

void WATCardOffice::main() {
}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
}

Job *WATCardOffice::requestWork() {
}
