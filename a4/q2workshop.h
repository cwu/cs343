#ifndef Q2WORKSHOP_H
#define Q2WORKSHOP_H

#include <uC++.h>

#include "q2printer.h"

_Monitor Workshop {
    Printer &printer;
    unsigned int elves;
    unsigned int bound;
    unsigned int deliveries;

    unsigned int elvesWaiting;
    unsigned int reindeersWaiting;

    unsigned int numWaitConsulting;
    unsigned int numWaitDelivering;

    unsigned int numTimesIgnoreElves;

    uCondition notEnoughElves;
    uCondition notEnoughReindeer;
    uCondition waitConsulting;
    uCondition waitDeliveries;
    uCondition santaNap;

    /**
     * checks if there are enough elves
     * returns true if we should dispatch the elves, false otherwise.
     */
    bool shouldDispatchElves();

    /**
     * check if there are enough reindeer and that we have not ignore the elves
     * too many times.
     *
     * returns true if we should dispatch the reindeer, false otherwise.
     */
    bool shouldDispatchReindeer();

    /**
     * Checks if the workshop is done.
     *
     * returns true if the workshop is done, false otherwise.
     */
    bool shouldCloseWorkshop();

    /**
     * Dispatch the waiting elves for consulting.
     */
    void dispatchElves();

    /**
     * Dispatch the waiting reindeer for deliveries.
     */
    void dispatchReindeer();
  public:
    enum Status { Consulting, Delivery, Done };
    Workshop( Printer &printer, unsigned int E, unsigned int N, unsigned int D ); // printer, elves, bound, deliveries
    Status sleep();                               // santa calls to nap; when Santa wakes status of next action
    void deliver( unsigned int id );              // reindeer call to deliver toys
    bool consult( unsigned int id );              // elves call to consult Santa,
                                                  //   true => consultation successful, false => consultation failed
    void doneConsulting( unsigned int id );       // block Santa/elves until meeting over
    void doneDelivering( unsigned int id );       // block Santa/reindeer until all toys are delivered
    void termination( unsigned int id );          // elves call to indicate termination
};

#endif // Q2WORKSHOP_H
