#include "q2northpole.h"
#include "MPRNG.h"

static const int REINDEER_START_YIELD = 10;
static const int REINDEER_YIELD = 3;
static const int REINDEER_VACATIONING_YIELD = 5;
static const int REINDEER_DELIVERING_YIELD = 5;

Reindeer::Reindeer( unsigned int id, Workshop &wrk, Printer &prt,
                    unsigned int numDeliveries )
                    : id(id), workshop(workshop), printer(printer),
                      numDeliveries(numDeliveries) {
}

void Reindeer::main() {
    // yield for stagged start
    yield(randomGen(REINDEER_START_YIELD));

    printer.print(id, Printer::Starting);
    for (unsigned int delivery = 0; delivery < numDeliveries; delivery++) {
        yield(randomGen(REINDEER_YIELD));

        // start working
        printer.print(id, Printer::OnVacation);
        yield(randomGen(REINDEER_VACATIONING_YIELD));

        // check in to deliver presents
        printer.print(id, Printer::CheckingIn);
        workshop.deliver(id);

        printer.print(id, Printer::DeliveringToys);
        yield(randomGen(REINDEER_DELIVERING_YIELD));

        // indicate done delivery
        workshop.doneDelivering(id);
        printer.print(id, Printer::DoneDelivering);
    }

    printer.print(id, Printer::Finished);
}
