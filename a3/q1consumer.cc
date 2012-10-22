#include "q1consumer.h"
#include "MPRNG.h"

void Consumer::main() {
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

Consumer::Consumer( BoundedBuffer<int> &buffer, const int Delay,
                    const int Sentinel, int &sum )
    : buffer(buffer), Delay(Delay), Sentinel(Sentinel), sum(sum) {
}
