#include <uC++.h>

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "MPRNG.h"
#include "q1printer.h"
#include "q1tallyVotes.h"
#include "q1voter.h"

using namespace std;
#include <cstdlib>

MPRNG randomGen;

bool convert(unsigned int &val, char *buffer ) {    // convert C string to integer
    std::stringstream ss( buffer );         // connect stream and buffer
    ss >> dec >> val;                       // convert integer from buffer
    return ! ss.fail() &&                   // conversion successful ?
                                            // characters after conversion all blank ?
            string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

/**
 * Prints the usage and exits the program.
 *
 * argv - the program argument values
 */
static void usage(char **argv) {
    cerr << "Usage: " << argv[0] << "  Voters (> 0 & V mod G = 0, default 6) "
         << "Group (> 0 & odd, default 3)  Seed (> 0)" << endl;

    exit(EXIT_FAILURE);
}

const unsigned int DEFAULT_VOTERS = 6;
const unsigned int DEFAULT_GROUPS = 3;


void uMain::main() {
    // too many arguments
    if (argc > 4) {
        usage(argv);
    }

    unsigned int V = DEFAULT_VOTERS;
    unsigned int G = DEFAULT_GROUPS;
    unsigned int seed = getpid();
    if ((argc >= 2 && (!convert(V, argv[1]) || V <= 0)) ||
        (argc >= 3 && (!convert(G, argv[2]) || G <= 0)) ||
        (argc >= 4 && (!convert(seed, argv[3]) || seed <= 0)) ||
        G % 2 != 1 || // odd group sizes
        V % G != 0) { // even groups
        usage(argv);
    }

    Printer printer(V);
    TallyVotes tallier(G, printer);
    Voter *voters[V];
    for (unsigned int voter = 0; voter < V; voter++) {
        voters[voter] = new Voter(voter, tallier, printer);
    }

    for (unsigned int voter = 0; voter < V; voter++) {
        delete voters[voter];
    }
}
