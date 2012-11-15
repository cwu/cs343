#include "q1voter.h"

#include "q1printer.h"
#include "MPRNG.h"

static const int VOTER_START_YIELD = 19;

Voter::Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer ) :
        id(id), voteTallier(voteTallier), printer(printer) {
}

void Voter::main() {
    // print starting message
    printer.print(id, Voter::Start);

    // don't all start at once
    yield(randomGen(VOTER_START_YIELD));

    // randomly cast a ballot
    bool ballot = randomGen(1) == 1;

    // submit vote
    voteTallier.vote(id, ballot);
}
