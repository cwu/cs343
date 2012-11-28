#include <uC++.h>
#include "parent.h"
#include "mprng.h"

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
  prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay)
{
  prt.print(Printer::Parent, (char)STARTING);
}

void Parent::main() {
  while (true) {
    _Accept(~Parent) {
      break;
    } else {
      yield(parentalDelay);
      int student_id = rng(numStudents - 1);
      int amount = rng(2) + 1;
      prt.print(Printer::Parent, (char)DEPOSIT, student_id, amount);
      bank.deposit(student_id, amount);
    }
  }

  prt.print(Printer::Parent, (char)FINISHED);
}
