#include "q1tallyVotes.h"

#include "q1voter.h"
#include "q1printer.h"

#include <assert.h>

#include <iostream>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer &printer )
    : group(group), printer(printer), votersForPictures(0), votersForStatues(0), numWaiting(0) {
}

bool TallyVotes::vote( unsigned int id, bool ballot ) {
    unsigned int *voteBucket = ballot ? &votersForPictures : &votersForStatues;
    unsigned int nessesaryTotalVotes = (totalVotes() / group + 1) * group;
    cout << nessesaryTotalVotes << endl;
    *voteBucket++;
    WAITUNTIL(
        totalVotes() >= nessesaryTotalVotes,
        numWaiting++;
        printer.print(id, Voter::Block, (unsigned int) numWaiting),
        numWaiting--;
        printer.print(id, Voter::Unblock, (unsigned int) totalVotes() - 2)
    );

    RETURN(false);
}
