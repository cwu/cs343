#ifndef Q1CONSUMER_H
#define Q1CONSUMER_H

#include <uC++.h>
#include "q1buffer.h"

_Task Consumer {
    BoundedBuffer<int> &buffer;
    const int Delay;
    const int Sentinel;
    int &sum;

    void main();
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel,
              int &sum );
};
#endif // Q1CONSUMER_H
