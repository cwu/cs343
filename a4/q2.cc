_Monitor Workshop {
    // private members go here
  public:
    enum Status { Consulting, Delivery, Done };
    Workshop( Printer &prt, unsigned int E, unsigned int N, unsigned int D ); // printer, elves, bound, deliveries
    Status sleep();                               // santa calls to nap; when Santa wakes status of next action
    void deliver( unsigned int id );              // reindeer call to deliver toys
    bool consult( unsigned int id );              // elves call to consult Santa,
                                                  //   true => consultation successful, false => consultation failed
    void doneConsulting( unsigned int id );       // block Santa/elves until meeting over
    void doneDelivering( unsigned int id );       // block Santa/reindeer until all toys are delivered
    void termination( unsigned int id );          // elves call to indicate termination
};

_Task Santa {
    // private members go here
  public:
    Santa( Workshop &wrk, Printer &prt );
};

_Task Elf {
    // private members go here
  public:
    enum { CONSULTING_GROUP_SIZE = 3 };           // number of elves for a consultation with Santa
    Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations );
};

_Task Reindeer {
    // private members go here
  public:
    enum { MAX_NUM_REINDEER = 5 };                // number of reindeer in system for delivering toys
    Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries );
};

_Monitor / _Cormonitor Printer {                  // choose monitor or cormonitor
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
};
