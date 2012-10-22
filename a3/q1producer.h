#ifndef Q1PRODUCER_H
#define Q1PRODUCER_H

#include <uC++.h>
#include "q1buffer.h"

_Task Producer {
    BoundedBuffer<int> &buffer;
    const int Produce;
    const int Delay;

    void main();
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay );
};

#endif // Q1PRODUCER_H
