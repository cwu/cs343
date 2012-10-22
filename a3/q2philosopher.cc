#include "q2philosopher.h"

#include "MPRNG.h"
#include "q2printer.h"
#include "q2table.h"

static const unsigned int HUNGRY_RANDOM = 4;
static const unsigned int NOODLE_RANDOM_LOWER = 1;
static const unsigned int NOODLE_RANDOM_UPPER = 5;
static const unsigned int EAT_RANDOM = 4;
static const unsigned int THINK_RANDOM = 19;

Philosopher::Philosopher( unsigned int id, unsigned int noodles, Table &table,
                          Printer &prt )
    : id(id), noodles(noodles), table(table), printer(prt) {
}


void Philosopher::main() {
    while (noodles > 0) {
        // get hungry
        printer.print(id, Hungry);
        yield(randomGen(HUNGRY_RANDOM));

        // pick up forks and eat a random amount of noodles
        table.pickup(id);
        unsigned int bite = randomGen(NOODLE_RANDOM_LOWER, NOODLE_RANDOM_UPPER);
        if (bite > noodles) {
            bite = noodles;
        }
        noodles -= bite;
        printer.print(id, Eating, bite, noodles);
        yield(randomGen(EAT_RANDOM));

        // put down the forks
        table.putdown(id);

        // think if we are not done eating yet
        if (noodles > 0) {
            printer.print(id, Thinking);
            yield(randomGen(THINK_RANDOM));
        }
    }
    printer.print(id, Finished);
}
