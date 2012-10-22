#include <uC++.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <unistd.h>

#include "q2table.h"
#include "q2philosopher.h"
#include "q2printer.h"
#include "MPRNG.h"

using namespace std;
#include <cstdlib>

const int DEFAULT_PHILOSOPHERS = 5;
const int DEFAULT_NOODLES = 30;

bool convert(int &val, char *buffer ) {    // convert C string to integer
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
    cerr << "Usage: " << argv[0] << " [ philosophers (> 1) [ noodles (> 0)"
         << " [ Seed (> 0) ] ] ]" << endl;
    exit(EXIT_FAILURE);
}

void uMain::main() {
    if (argc > 4) {
        usage(argv);
    }

    // Parse command line
    int numPhilosophers = DEFAULT_PHILOSOPHERS;
    if (argc >= 2 && (!convert(numPhilosophers, argv[1]) || numPhilosophers <= 1)) {
        usage(argv);
    }

    int numNoodles = DEFAULT_NOODLES;
    if (argc >= 3 && (!convert(numNoodles, argv[2]) || numNoodles <= 0)) {
        usage(argv);
    }

    int seed;
    if (argc >= 4 && (!convert(seed, argv[3]) || seed <= 0)) {
        usage(argv);
    } else if (argc < 4) {
        seed = getpid();
    }

    randomGen.seed(seed);

    Printer printer(numPhilosophers);
    Table table(numPhilosophers, printer);
    Philosopher *philosophers[numPhilosophers];
    for (int id = 0; id < numPhilosophers; id++) {
        philosophers[id] = new Philosopher(id, numNoodles, table, printer);
    }

    for (int id = 0; id < numPhilosophers; id++) {
        delete philosophers[id];
    }
}
