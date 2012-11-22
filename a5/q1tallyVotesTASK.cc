#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"

#include <iostream>

using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer &printer )
    : done(false), group(group), printer(printer), votersForPictures(0),
      votersForStatues(0), numNeededVoters(group) {
}

TallyVotes::~TallyVotes() {
    // signal done and block until the main finishes first
    done = true;
    serverNeedsWork.signalBlock();
}

void TallyVotes::main() {
    while (true) {
        if (workQueue.empty()) {
            serverNeedsWork.wait();
        }

        if (done) {
            break;
        }

        // dequeue the work
        pair<int, bool> work = workQueue.front();
        workQueue.pop();

        int id = work.first;
        bool ballot = work.second;

        // keep track of number of needed voters
        numNeededVoters--;
        printer.print(id, Voter::Block, group - numNeededVoters);

        // keep track of the vote
        if (ballot) {
            votersForPictures++;
        } else {
            votersForStatues++;
        }

        // check for completion
        if (numNeededVoters == 0) {
            printer.print(id, Voter::Complete);

            // determine the winner
            winner = votersForPictures > votersForStatues;

            // reset the votes
            votersForPictures = 0;
            votersForStatues = 0;

            // signal all of the voters
            for (unsigned int v = 0; v < group; v++) {
                needVoters.signal();
            }
        }
    }

    // signal back to destructor to finish
    serverNeedsWork.signal();
}

bool TallyVotes::vote( unsigned int id, bool ballot ) {

    // enqueue the work
    workQueue.push(make_pair(id, ballot));

    // signal that there is work to do and block until all voters are in
    serverNeedsWork.signal();
    needVoters.wait();

    // clean up the number of needed voters and print unblock
    numNeededVoters++;
    printer.print(id, Voter::Unblock, group - numNeededVoters);
    return winner;
}
