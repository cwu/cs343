#include "q2santa.h"
#include "MPRNG.h"

static const int SANTA_START_YIELD = 10;
static const int SANTA_YIELD = 3;
static const int SANTA_CONSULT_YIELD = 3;
static const int SANTA_DELIVERY_YIELD = 5;

Santa::Santa(Workshop &workshop, Printer& printer)
    : workshop(workshop), printer(printer) {
}

void Santa::main() {
    // yield for stagged start
    yield(randomGen(SANTA_START_YIELD));

    // print start message
    printer.print(SANTA_ID, Printer::Starting);

    while (true) {
        yield(randomGen(SANTA_YIELD));

        // nap until awoken
        printer.print(SANTA_ID, Printer::Napping);
        Workshop::Status status = workshop.sleep();
        printer.print(SANTA_ID, Printer::Awake);

        if (status == Workshop::Done) {
            break;
        } else if (status == Workshop::Consulting) {
            yield(randomGen(SANTA_CONSULT_YIELD));
            workshop.doneConsulting(SANTA_ID);
            printer.print(SANTA_ID, Printer::DoneConsulting);
        } else if (status == Workshop::Delivery) {
            yield(randomGen(SANTA_DELIVERY_YIELD));
            workshop.doneDelivering(SANTA_ID);
            printer.print(SANTA_ID, Printer::DoneDelivering);
        }
    }

    printer.print(SANTA_ID, Printer::Finished);
}
