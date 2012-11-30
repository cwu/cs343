#include <uC++.h>

#include <unistd.h>

#include <iostream>
#include <sstream>

#include <cstdlib>

#include "config.h"

using namespace std;

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
    cerr << "Usage: " << argv[0] << " [ config-file [ random-seed (> 0) ] ]"
         << endl;
    exit(EXIT_FAILURE);
}

void uMain::main() {
    // too many parameters
    if (argc >= 3) {
        usage(argv);
    }

    char* configFile = "soda.config";
    int seed = getpid();
    if (argc >= 2) {
        configFile = argv[1];
    }
    if (argc >= 3 && (!convert(seed, argv[3]) || seed <= 0)) {
        usage(argv);
    }

    ConfigParms params;
    processConfigFile(configFile, params);
}
