#include <uC++.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "q1buffer.h"
#include "MPRNG.h"

using namespace std;
#include <cstdlib>

const int DEFAULT_CONS = 5;
const int DEFAULT_PRODS = 3;
const int DEFAULT_PRODUCE = 10;
const int DEFAULT_BUFFER_SIZE = 10;

static MPRNG randomGen;

bool convert(int &val, char *buffer ) {    // convert C string to integer
    std::stringstream ss( buffer );         // connect stream and buffer
    ss >> dec >> val;                       // convert integer from buffer
    return ! ss.fail() &&                   // conversion successful ?
                                            // characters after conversion all blank ?
            string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

/**
 * Prints the usage and exits the program.
 *
 * argv - the program argument values
 */
static void usage(char **argv) {
    cerr << "Usage: " << argv[0] << " [ Cons (> 0) [ Prods (> 0) [ Produce (> 0) "
         << "[ BufferSize (> 0) [ Delay (> 0) ] ] ] ] ]" << endl;
    exit(EXIT_FAILURE);
}

_Task Producer {
    BoundedBuffer<int> &buffer;
    const int Produce;
    const int Delay;

    void main() {
        for (int produce = 1; produce <= Produce; produce++) {
            yield(randomGen(Delay));
            buffer.insert(produce);
        }
    }
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay )
        : buffer(buffer), Produce(Produce), Delay(Delay) {}
};

_Task Consumer {
    BoundedBuffer<int> &buffer;
    const int Delay;
    const int Sentinel;
    int &sum;

    void main() {
        yield(randomGen(Delay));
        int value = buffer.remove();
        while (value != Sentinel) {
            // accumulate sum only after we know it is not the Sentinel value
            sum += value;

            // get a new value
            yield(randomGen(Delay));
            value = buffer.remove();
        }
    }
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel,
              int &sum )
        : buffer(buffer), Delay(Delay), Sentinel(Sentinel), sum(sum) {}
};

void uMain::main() {
    if (argc > 6) {
        usage(argv);
    }

    // Parse command line
    int cons = DEFAULT_CONS;
    if (argc >= 2 && (!convert(cons, argv[1]) || cons <= 0)) {
        usage(argv);
    }

    int prods = DEFAULT_PRODS;
    if (argc >= 3 && (!convert(prods, argv[2]) || prods <= 0)) {
        usage(argv);
    }

    int produce = DEFAULT_PRODUCE;
    if (argc >= 4 && (!convert(produce, argv[3]) || produce <= 0)) {
        usage(argv);
    }

    int bufferSize = DEFAULT_BUFFER_SIZE;
    if (argc >= 5 && (!convert(bufferSize, argv[4]) || bufferSize <= 0)) {
        usage(argv);
    }

    int delays = cons + prods;
    if (argc >= 6 && (!convert(delays, argv[5]) || delays <= 0)) {
        usage(argv);
    }

#ifdef __U_MULTI__
    uProcessor p[3] __attribute__((unused));
#endif // __U_MULTI__

    // Setup the tasks
    BoundedBuffer<int> buffer(bufferSize);

    Producer *producers[prods];
    for (int prod = 0; prod < prods; prod++) {
        producers[prod] = new Producer(buffer, produce, delays);
    }

    Consumer *consumers[cons];
    int partialSums[cons];
    int sentinel = -1;
    for (int con = 0; con < cons; con++) {
        partialSums[con] = 0;
        consumers[con] = new Consumer(buffer, delays, sentinel,
                                      partialSums[con]);
    }

    // Wait for producers to finish
    for (int prod = 0; prod < prods; prod++) {
        delete producers[prod];
    }

    // insert sentinel values into the buffer and wait for consumers to finish
    for (int con = 0; con < cons; con++) {
        buffer.insert(sentinel);
    }
    for (int con = 0; con < cons; con++) {
        delete consumers[con];
    }

    // sum the partial sums
    int sum = 0;
    for (int con = 0; con < cons; con++) {
        sum += partialSums[con];
    }
    cout << "total: " << sum << endl;
}
