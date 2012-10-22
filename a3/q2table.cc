#include "q2table.h"

Table::Table( unsigned int noOfPhil, Printer &prt )
    : numOfPhil(noOfPhil),
      printer(prt),
      philWait(new uSemaphore*[numOfPhil]),
      philStates(new State[numOfPhil]) {
    for (unsigned int id = 0; id < numOfPhil; id++) {
        philStates[id] = Thinking;
        philWait[id] = new uSemaphore(0);
    }
}

Table::~Table() {
    for (unsigned int id = 0; id < numOfPhil; id++) {
        delete philWait[id];
    }
    delete [] philWait;
    delete [] philStates;
}

void Table::pickup( unsigned int id ) {
    // try to pick up the even fork then the odd fork
    mutex.P();

    // If either philosopher beside us is eating then we do not have the
    // forks to eat. So wait.
    if (philStates[left(id)] == Eating || philStates[right(id)] == Eating) {
        philStates[id] = Waiting;
        printer.print(id, Philosopher::Waiting);
        philWait[id]->P(mutex);
        mutex.P(mutex);
    } else {
        assert(philStates[left(id)] != Eating && philStates[right(id)] != Eating);
        mutex.V();
    }

    // Perform integrity checking to ensure no philosophers next to us are
    // eating since we are using both forks.
    mutex.P();
    assert(philStates[left(id)] != Eating && philStates[right(id)] != Eating);
    mutex.V();
}

void Table::putdown( unsigned int id ) {
    mutex.P();

    // put down our forks
    philStates[id] = Thinking;

    // Check if the philosopher left of us is waiting and to their left is not eating.
    // then they may eat now
    if (philStates[left(left(id))] != Eating && philStates[left(id)] == Waiting) {
        // simulate picking up both forks and eating
        philStates[id] = Eating;
        philWait[left(id)]->V();
    }

    // Check if the philosopher right of us is waiting and to their right is not eating.
    // then they may eat now
    if (philStates[right(right(id))] != Eating && philStates[right(id)] == Waiting) {

        // simulate picking up both forks and eating
        philStates[id] = Eating;
        philWait[right(id)]->V();
    }

    mutex.V();
}

unsigned int Table::left(unsigned int id) {
    return (id + (numOfPhil - 1)) % numOfPhil;
}

unsigned int Table::right(unsigned int id) {
    return (id + 1) % numOfPhil;
}
