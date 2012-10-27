#ifndef Q2PRINTER_H
#define Q2PRINTER_H

#include <uC++.h>

_Monitor Printer {                  // choose monitor or cormonitor
  public:
    enum States { Starting = 'S', Blocked = 'B', Unblocked = 'U', Finished = 'F', // general
          Napping = 'N', Awake = 'A',             // Santa
          Working = 'W', NeedHelp = 'H',          // elf
          OnVacation = 'V', CheckingIn = 'I',     // reindeer
          DeliveringToys = 'D', DoneDelivering = 'd', // Santa, reindeer
          Consulting = 'C', DoneConsulting = 'c', // Santa, elves
          ConsultingFailed = 'X',                 // elf
    };
    Printer( const unsigned int MAX_NUM_ELVES );
    void print( unsigned int id, States state );
    void print( unsigned int id, States state, unsigned int numBlocked );
  private:
    unsigned int numElves;
    unsigned int numIds;
    struct PrintState {
        bool hasData;
        States state;
        unsigned int numBlocked;
    };
    PrintState *printStates;

    void printFinished(unsigned int finishedId);
    void flushStates();
};

#endif // Q2PRINTER_H
