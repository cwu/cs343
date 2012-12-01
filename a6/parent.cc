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
      int studentId = rng(numStudents - 1);
      int amount = rng(1, 3);
      prt.print(Printer::Parent, (char)DEPOSIT, studentId, amount);
      bank.deposit(studentId, amount);
    }
  }

  prt.print(Printer::Parent, (char)FINISHED);
}
