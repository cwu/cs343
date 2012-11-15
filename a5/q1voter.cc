#include "q1voter.h"

Voter::Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer ) :
        id(id), voteTallier(voteTallier), printer(printer) {
}
