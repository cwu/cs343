#include "q2northpole.h"
#include "MPRNG.h"

static const int ELF_START_YIELD = 10;
static const int ELF_YIELD = 3;
static const int ELF_WORKING_YIELD = 5;
static const int ELF_CONSULTING_YIELD = 3;

Elf::Elf( unsigned int id, Workshop &workshop, Printer &printer,
          unsigned int numConsultations )
        : id(id), workshop(workshop), printer(printer),
          numConsultations(numConsultations) {
}


void Elf::main() {
    // yield for stagged start
    yield(randomGen(ELF_START_YIELD));

    printer.print(id, Printer::Starting);
    for (unsigned int consultation = 0; consultation < numConsultations; consultation++) {
        yield(randomGen(ELF_YIELD));

        // start working
        printer.print(id, Printer::Working);
        yield(randomGen(ELF_WORKING_YIELD));

        // get help from Santa
        printer.print(id, Printer::NeedHelp);
        bool successful = workshop.consult(id);
        if (!successful) {
            printer.print(id, Printer::ConsultingFailed);
            break;
        }

        // finish the consulting
        printer.print(id, Printer::Consulting);
        yield(randomGen(ELF_CONSULTING_YIELD));
        workshop.doneConsulting(id);
        printer.print(id, Printer::DoneConsulting);
    }

    workshop.termination(id);
    printer.print(id, Printer::Finished);
}

