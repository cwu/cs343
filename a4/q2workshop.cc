#include "q2workshop.h"

#include <assert.h>

#include "q2santa.h"
#include "q2elf.h"
#include "q2reindeer.h"

Workshop::Workshop( Printer &printer, unsigned int E, unsigned int N, unsigned int D )
    : printer(printer),
      elves(E),
      bound(N),
      deliveries(D),
      elvesWaiting(0),
      reindeersWaiting(0),
      numWaitConsulting(0),
      numWaitDelivering(0),
      numTimesIgnoreElves(0) {
}

Workshop::Status Workshop::sleep() {
    // return immediately if we have enough reindeers or elves waiting
    if (shouldDispatchReindeer()) {
        dispatchReindeer();
        return Delivery;
    } else if (shouldDispatchElves()) {
        dispatchElves();
        return Consulting;
    } else if (shouldCloseWorkshop()) {
        return Done;
    }

    // block and wait for elves or reindeer to be ready
    printer.print(SANTA_ID, Printer::Blocked);
    santaNap.wait();

    // dispatch santa to deliver or consult depending on who is ready
    if (shouldDispatchReindeer()) {
        dispatchReindeer();
        return Delivery;
    } else if (shouldDispatchElves()) {
        dispatchElves();
        return Consulting;
    }

    // if we woke up and the reindeer and elves are ready it means we are done
    assert(shouldCloseWorkshop());
    return Done;
}

void Workshop::deliver( unsigned int id ) {
    reindeersWaiting++;

    // there are enough reindeer for deliveries so wake up santa to start the
    // deliveries
    if (reindeersWaiting >= Reindeer::MAX_NUM_REINDEER) {
        santaNap.signal();
    }

    // wait for santa
    printer.print(id, Printer::Blocked, reindeersWaiting);
    notEnoughReindeer.wait();
    printer.print(id, Printer::Unblocked, reindeersWaiting);
}

bool Workshop::consult( unsigned int id ) {
    // make sure there are enough elves to consult with
    if (elves < Elf::CONSULTING_GROUP_SIZE) {
        return false;
    }

    elvesWaiting++;

    // there are enough reindeer for deliveries so wake up santa to start the
    // deliveries
    if (elvesWaiting >= Elf::CONSULTING_GROUP_SIZE) {
        santaNap.signal();
    }

    // wait for santa
    printer.print(id, Printer::Blocked, elvesWaiting);
    notEnoughElves.wait();
    printer.print(id, Printer::Unblocked, elvesWaiting);

    // make sure there are enough elves to consult with in case
    // we woke up because of terminate
    if (elves < Elf::CONSULTING_GROUP_SIZE) {
        return false;
    }
    return true;
}

void Workshop::doneConsulting( unsigned int id ) {
    numWaitConsulting++;

    // Wait until all the elves and santa are done consulting
    if (numWaitConsulting < Elf::CONSULTING_GROUP_SIZE + 1) {
        printer.print(id, Printer::Blocked, numWaitConsulting);
        waitConsulting.wait();
        printer.print(id, Printer::Unblocked, numWaitConsulting);
    } else {
        // last one done should signal the otheres
        for (int i = 0; i < Elf::CONSULTING_GROUP_SIZE; i++) {
            waitConsulting.signal();
        }
    }
    numWaitConsulting--;
    assert(numWaitConsulting >= 0);
}

void Workshop::doneDelivering( unsigned int id ) {
    numWaitDelivering++;

    // wait until the reindeers and santa are done delivering
    if (numWaitDelivering < Reindeer::MAX_NUM_REINDEER + 1) {
        printer.print(id, Printer::Blocked, numWaitDelivering);
        waitDeliveries.wait();
        printer.print(id, Printer::Unblocked, numWaitDelivering);
    } else {
        // last one done should signal the otheres
        for (int i = 0; i < Reindeer::MAX_NUM_REINDEER; i++) {
            waitDeliveries.signal();
        }

        // keep track of all the deliveries we've done
        deliveries--;

        // check if we're done the deliveries and the elves are done
        if (shouldCloseWorkshop()) {
            santaNap.signal();
        }
    }
    numWaitDelivering--;
    assert(numWaitDelivering >= 0);
}

void Workshop::termination( unsigned int id ) {
    elves--;

    // if we were the last elf to terminate we should wake santa to finish
    // the workshop
    if (shouldCloseWorkshop()) {
        santaNap.signal();
    }

    // signal any elves waiting to consult if there are not enough elves
    // left to consult with
    if (elves < Elf::CONSULTING_GROUP_SIZE && !notEnoughElves.empty()) {
        for (; elvesWaiting > 0; elvesWaiting--) {
            notEnoughElves.signal();
        }
    }
}

bool Workshop::shouldDispatchElves() {
    return elvesWaiting >= Elf::CONSULTING_GROUP_SIZE;
}

bool Workshop::shouldDispatchReindeer() {
    return reindeersWaiting >= Reindeer::MAX_NUM_REINDEER &&
           !(numTimesIgnoreElves >= bound && shouldDispatchElves());
}

bool Workshop::shouldCloseWorkshop() {
    return deliveries == 0 && elves == 0;
}

void Workshop::dispatchElves() {
    // clear the number of times the elves are ignored
    numTimesIgnoreElves = 0;

    for (int i = 0; i < Elf::CONSULTING_GROUP_SIZE; i++) {
        notEnoughElves.signal();
    }
    elvesWaiting -= Elf::CONSULTING_GROUP_SIZE;
    assert(elvesWaiting >= 0);
}

void Workshop::dispatchReindeer() {
    // keep track of how many times we dispatch the reindeer when elves are
    // ready
    if (shouldDispatchElves()) {
        numTimesIgnoreElves++;
    }

    for (int i = 0; i < Reindeer::MAX_NUM_REINDEER; i++) {
        notEnoughReindeer.signal();
    }
    reindeersWaiting -= Reindeer::MAX_NUM_REINDEER;
    assert(reindeersWaiting >= 0);
}
