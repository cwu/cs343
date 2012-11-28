#ifndef __A6OFFICE_H__
#define __A6OFFICE_H__

#include <uC++.h>
#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {
  private:
    struct Args {
      unsigned int sid;
      Bank &bank;
      WATCard *card;
      unsigned int amount;

      Args(Bank &bank, unsigned int sid, WATCard *card, unsigned int amount) :
        bank(bank), sid(sid), card(card), amount(amount) {}
    };

    struct Job {				// marshalled arguments and return future
      Args args;				// call arguments (YOU DEFINE "Args")
      FWATCard result;			// return future

      Job( Args args ) : args( args ) {}
    };

		// communicates with bank
    _Task Courier {
      private:
        unsigned int id;
        WATCardOffice *office;
        Printer &prt;

        void main();

      public:
        enum State {STARTING = 'S', START_XFER = 't', END_XFER = 'T', FINISHED = 'F'};

        Courier(unsigned int id, WATCardOffice *office, Printer &prt) : id(id), office(office), prt(prt) {}
    };

    Printer &prt;
    Bank &bank;
    unsigned int numCouriers;
    Courier **couriers;
    uCondition jobQ;
    Job *pJob;
    bool isDone;


    void main();
  public:
    enum State {STARTING = 'S', COURIER_COMPLETE = 'W', CREATION_COMPLETE = 'C',
      XFER_COMPLETE = 'T', FINISHED = 'F'};
    _Event Lost {};

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    FWATCard create( unsigned int sid, unsigned int amount );
    FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
