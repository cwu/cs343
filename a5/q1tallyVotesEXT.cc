#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"

TallyVotes::TallyVotes( unsigned int group, Printer &printer )
    : group(group), printer(printer), votersForPictures(0), votersForStatues(0),
      numNeededVoters(group) {
}

bool TallyVotes::vote( unsigned int id, bool ballot ) {
    // keep track of number of needed voters
    numNeededVoters--;

    // keep track of the vote
    if (ballot) {
        votersForPictures++;
    } else {
        votersForStatues++;
    }

    // if we don't have enough voters then block
    if (numNeededVoters > 0) {
        printer.print(id, Voter::Block, group - numNeededVoters);
        _Accept(vote);
        numNeededVoters++; // unmark the number of needed voters
        printer.print(id, Voter::Unblock, group - numNeededVoters);
    } else {
        printer.print(id, Voter::Complete);

        numNeededVoters++; // unmark the number of needed voters

        // determine the winner
        winner = votersForPictures > votersForStatues;

        // reset the votes
        votersForPictures = 0;
        votersForStatues = 0;
    }

    return winner;
}
