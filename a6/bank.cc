#include "bank.h"

Bank::Bank( unsigned int numStudents ) : students(numStudents) {
  accounts = new int [students];
  account_conds = new uCondition [students];
  for (unsigned int i = 0; i < students; i++) {
    accounts[i] = 0;
  }
}

Bank::~Bank() {
  delete [] accounts;
  delete [] account_conds;
}

// Single parent drops money off for all the children of UWaterloo
void Bank::deposit( unsigned int id, unsigned int amount ) {
  accounts[id] += amount;
  account_conds[id].signal(); // TODO: will one signal suffice? or signal until empty?
}

// courier calls to get funds for watcard
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while (accounts[id] < amount) account_conds[id].wait();

  accounts[id] -= amount;
}
