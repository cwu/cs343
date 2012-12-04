#include "printer.h"
#include <iostream>

const unsigned int SUM_OF_SINGLE_KINDS = 5;
const int NO_VALUE = -1;

using namespace std;

// Ctor, initialises member fields and prints header
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
  students(numStudents), machines(numVendingMachines), couriers(numCouriers) {
  total_size = SUM_OF_SINGLE_KINDS + students + machines + couriers;
  entries = new Entry [total_size];

  // Print header
  cout << "Parent\tWATOff\tNames\tTruck\tPlant\t";
  for (unsigned int i = 0; i < students; i++) {
    cout << "Stud" << i << '\t';
  }
  for (unsigned int i = 0; i < machines; i++) {
    cout << "Mach" << i << '\t';
  }
  for (unsigned int i = 0; i < couriers; i++) {
    cout << "Cour" << i << '\t';
  }
  cout << endl;
  for (unsigned int i = 0; i < total_size; i++) {
    cout << "=======" << '\t';
  }
  cout << endl;

  // Initialise entries
  for (unsigned int i = 0; i < total_size; i++) {
    entries[i].filled = false;
  }
}

// Print footer, free memory
Printer::~Printer () {
  cout << "***********************" << endl;
  delete [] entries;
}

_Mutex void Printer::print( unsigned int id, char state, int value1, int value2) {
  if (state == 'F') {
      // check if we need to flush beforehand
      for (unsigned int i = 0; i < total_size; i++) {
          if (entries[id].filled) {
            flush();
            break;
          }
      }

      // print the finished state
      for (unsigned int i = 0; i < total_size; i++) {
        if (id != i)
            cout << "...";
        else
            cout << state;
        cout << "\t";
      }
      cout << endl;
  } else if (entries[id].filled) {
      flush();
  }

  if (state != 'F') {
    entries[id].state  = state;
    entries[id].value1 = value1;
    entries[id].value2 = value2;
    entries[id].filled = true;
  }
}

void Printer::print( Kind kind, char state ) {
  print(kind, state, NO_VALUE, NO_VALUE);
}

void Printer::print( Kind kind, char state, int value1 ) {
  print(kind, state, value1, NO_VALUE);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
  print(kind, 0, state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
  print(kind, lid, state, NO_VALUE, NO_VALUE);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
  print(kind, lid, state, value1, NO_VALUE);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
  int offset = 0;
  switch (kind) {
    case Student:
      offset = SUM_OF_SINGLE_KINDS;
      break;
    case Vending:
      offset = SUM_OF_SINGLE_KINDS + students;
      break;
    case Courier:
      offset = SUM_OF_SINGLE_KINDS + students + machines;
      break;
    default:
      offset = (unsigned int) kind;
      break;
  }

  print(offset + lid, state, value1, value2);
}

// Flush buffered data to console output
void Printer::flush () {
  for (unsigned int i = 0; i < total_size; i++) {
    if (entries[i].filled) {
      cout << entries[i].state;
      if (entries[i].value1 != NO_VALUE) {
        cout << entries[i].value1;
        if (entries[i].value2 != NO_VALUE) {
          cout << ',' << entries[i].value2;
        }
      }
      entries[i].filled = false;
    }
    cout << "\t";
  }
  cout << endl;
}
