#include "q1producer.h"
#include "MPRNG.h"

void Producer::main() {
    for (int produce = 1; produce <= Produce; produce++) {
        yield(randomGen(Delay));
        buffer.insert(produce);
    }
}

Producer::Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay )
    : buffer(buffer), Produce(Produce), Delay(Delay) {}
