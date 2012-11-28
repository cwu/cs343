#ifndef __A6PRINTER_H__
#define __A6PRINTER_H__

#include <uC++.h>

_Monitor Printer {
  private:
    struct Entry {
      char state;
      int value1, value2;
      unsigned int id;
      bool filled;
    };

    unsigned int students, machines, couriers, total_size;
    Entry *entries;

    void flush();
    _Mutex void print(unsigned int id, char state, int value1, int value2);

  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();

    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif
