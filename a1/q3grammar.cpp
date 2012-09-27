#include "q3grammar.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/**
 * Perform the coroutine to match the grammer of 'ab+c*d' where
 * there is an additional constraint that the number of b's is
 * 1 greater than the number of c's.
 *
 * The coroutine gets input from the member variable 'ch' and
 * appropriately sets the member variable status to the current
 * matching status.
 */
void Grammar::main() {
    // Check for the a
    if (ch == 'a') {
        status = CONT;
    } else {
        status = ERROR;
        return;
    }
    suspend();

    // Ensure there is at least one b
    if (ch != 'b') {
        status = ERROR;
        return;
    }

    // Count the number of b's
    int numberOfB = 0;
    while (ch == 'b') {
        numberOfB++;
        suspend();
    }

    // Counter the number of c's
    int numberOfC = 0;
    while (ch == 'c') {
        numberOfC++;
        if (numberOfC == numberOfB) {
            status = ERROR;
            return;
        }
        suspend();
    }

    // Ensure there is a d character and the number of b's is the same as
    // the number of c's + 1. Note: these two conditions guarantee that
    // there was no other character inbetween the counting of the b's
    // and c's and the d so it would default to an error match.
    if (ch == 'd' && numberOfB == numberOfC + 1) {
        status = MATCH;
    } else {
        status = ERROR;
    }
}

void uMain::main() {
    // Use a file if specifed, otherwise read from stdin
    istream *input;
    bool useFile = argc >= 2;
    if (useFile) {
        input = new ifstream(argv[1]);
    } else {
        input = &cin;
    }

    string line;
    while (getline(*input, line)) {
        if (line.empty()) {
            cout << "\"\" : Warning! Blank line." << endl;
            continue;
        }

        Grammar grammar;
        Grammar::Status status;
        string extra;
        for (unsigned int i = 0; i < line.length(); i++) {
            status = grammar.next(line[i]);
            if (status != Grammar::CONT) {
                extra = line.substr(i+1);
                break;
            }
        }

        string consumed(line, 0, line.size() - extra.size());

        // output the results
        cout << "\"" << line << "\" : \"" << consumed << "\" ";
        if (status == Grammar::MATCH) {
            cout << "yes";
        } else {
            cout << "no";
        }
        if (!extra.empty()) {
            cout << " -- extraneous characters \"" << extra << "\"";
        }
        cout << endl;
    }

    if (useFile) {
        delete input;
    }
}
