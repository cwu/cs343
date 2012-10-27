#include <uC++.h>

#include <unistd.h>

#include <iostream>
#include <sstream>
#include "q2northpole.h"
#include "q2printer.h"
#include "q2workshop.h"
#include "q2santa.h"
#include "q2elf.h"
#include "q2reindeer.h"

using namespace std;
#include <cstdlib>

const int DEFAULT_N = 3;
const int DEFAULT_E = 3;
const int DEFAULT_C = 3;
const int DEFAULT_D = 3;

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
    cerr << "Usage: " << argv[0] << " [ N (> 0) [ E (> 0) [ seed (> 0) [ C "
         << "(>= 0) [ D (>= 0) ] ] ] ] ]" << endl;
    exit(EXIT_FAILURE);
}

void uMain::main() {
    // too many arguments
    if (argc > 6) {
        usage(argv);
    }

    // parse arumgents
    int N = DEFAULT_N;
    int E = DEFAULT_E;
    int seed = getpid();
    int C = DEFAULT_C;
    int D = DEFAULT_D;
    if ((argc >= 2 && (!convert(N, argv[1]) || N <= 0)) ||
        (argc >= 3 && (!convert(E, argv[2]) || N <= 0)) ||
        (argc >= 4 && (!convert(seed, argv[3]) || seed <= 0)) ||
        (argc >= 5 && (!convert(C, argv[4]) || C < 0)) ||
        (argc >= 6 && (!convert(D, argv[5]) || D < 0))) {
        usage(argv);
    }

    Printer printer(E);
    Workshop workshop(printer, E, N, D);
    Santa *santa = new Santa(workshop, printer);
    Elf *elves[E];
    for (int elf = 0; elf < E; elf++) {
        elves[elf] = new Elf(elf + 1, workshop, printer, C);
    }

    Reindeer *reindeers[Reindeer::MAX_NUM_REINDEER];
    for (int reindeer = 0; reindeer < E; reindeer++) {
        reindeers[reindeer] = new Reindeer(reindeer + 1 + E, workshop, printer, D);
    }
}
