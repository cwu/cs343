#include <uC++.h>

#include <unistd.h>

#include <iostream>
#include <sstream>

#include <cstdlib>

#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "office.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"
#include "mprng.h"

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

    // parse config file params
    string configFile = "soda.config";
    if (argc >= 2) {
        configFile = string(argv[1]);
    }

    // parse seed
    int seed = getpid();
    if (argc >= 3 && (!convert(seed, argv[3]) || seed <= 0)) {
        usage(argv);
    }
    rng.seed(seed);

    ConfigParms params;
    processConfigFile(configFile.c_str(), params);

    Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
    Bank bank(params.numStudents);
    Parent *parent = new Parent(printer, bank, params.numStudents, params.parentalDelay);
    WATCardOffice *office = new WATCardOffice(printer, bank, params.numCouriers);
    NameServer * nameserver = new NameServer(printer, params.numVendingMachines, params.numStudents);
    VendingMachine *vendingMachines[params.numVendingMachines];
    for (unsigned int machine = 0; machine < params.numVendingMachines; machine++) {
        vendingMachines[machine] = new VendingMachine(printer, *nameserver, machine, params.sodaCost, params.maxStockPerFlavour);
    }
    BottlingPlant *plant = new BottlingPlant(printer, *nameserver, params.numVendingMachines, params.maxShippedPerFlavour, params.maxStockPerFlavour, params.timeBetweenShipments);

    Student *students[params.numStudents];
    for (unsigned int student = 0; student < params.numStudents; student++) {
        students[student] = new Student(printer, *nameserver, *office, student, params.maxPurchases);
    }

    // delete tasks in reverse order
    for (unsigned int student = 0; student < params.numStudents; student++) {
        delete students[student];
    }

    delete plant;
    for (unsigned int machine = 0; machine < params.numVendingMachines; machine++) {
        delete vendingMachines[machine];
    }

    delete nameserver;
    delete office;
    delete parent;
}
