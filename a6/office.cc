#include <uC++.h>
#include "office.h"
#include "mprng.h"

#define INV_LOSE_CARD_CHANCE 6

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
  prt(prt), bank(bank), numCouriers(numCouriers), pJob(NULL), isDone(false)
{
  couriers = new Courier*[numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i] = new Courier(i, this, prt);
  }

  prt.print(Printer::WATCardOffice, (char)STARTING);
}

WATCardOffice::~WATCardOffice() {
  prt.print(Printer::WATCardOffice, (char)FINISHED);

  //Stop couriers
  isDone = true;
  pJob = NULL;
  while (!jobQ.empty()) jobQ.signal();

  for (unsigned int i = 0; i < numCouriers; i++) {
    delete couriers[i];
  }
  delete [] couriers;
}


void WATCardOffice::main() {
  while(true) {
    _Accept(~WATCardOffice) break;
    or _When (!jobQ.empty()) _Accept(create, transfer);
  }
}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  // TODO: so wtf deletes the jobs?
  pJob = new Job(Args(bank, sid, NULL, amount));

  jobQ.signal();
  prt.print(Printer::WATCardOffice, (char)CREATION_COMPLETE, sid, amount);

  return pJob->result;
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  pJob = new Job(Args(bank, sid, card, amount));

  jobQ.signal();
  prt.print(Printer::WATCardOffice, (char)XFER_COMPLETE, sid, amount);

  return pJob->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
  if (isDone) return NULL;
  jobQ.wait();
  return pJob;
}

void WATCardOffice::Courier::main() {
  prt.print(Printer::Courier, id, (char)STARTING);
  while (true) {
    Job *job = office->requestWork();
    Args args = job->args;

    // Check for exit
    if (job == NULL) break;

    // Chance to drop card
    if (rng(INV_LOSE_CARD_CHANCE - 1) == 0) {
      job->result.exception(new Lost);
      if (args.card != NULL) delete args.card;
      continue;
    }

    prt.print(Printer::Courier, id, (char) START_XFER, args.sid, args.amount);

    // Withdraw from bank (block if necessary) then
    // add funds to card (create new if necessary)
    args.bank.withdraw(args.sid, args.amount);
    if (args.card == NULL) args.card = new WATCard();
    args.card->deposit(args.amount);
    prt.print(Printer::Courier, id, (char) END_XFER, args.sid, args.amount);

    job->result.delivery(args.card);
  }
  prt.print(Printer::Courier, id, (char)FINISHED);
}