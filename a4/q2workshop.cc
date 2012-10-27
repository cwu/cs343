#include "q2workshop.h"

Workshop::Workshop( Printer &prt, unsigned int E, unsigned int N, unsigned int D ) :
    printer(prt), elves(E), bound(N), deliveries(D) {
}

Workshop::Status Workshop::sleep() {
    return Done;
}

void Workshop::deliver( unsigned int id ) {
}

bool Workshop::consult( unsigned int id ) {
    return true;
}

void Workshop::doneConsulting( unsigned int id ) {
}

void Workshop::doneDelivering( unsigned int id ) {
}

void Workshop::termination( unsigned int id ) {
}
