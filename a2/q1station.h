#ifndef __STATION_H__
#define __STATION_H__

#include <list>
#include <uC++.h>

_Coroutine Station {

  private:
    struct Frame {
        enum { Token, Data, Ack } type;
        unsigned int src;
        unsigned int dst;
        unsigned int prio;
    } frame; // Frame

    void data( Frame frame );				// pass frame
    void main();
    bool hasPendingRequest( unsigned int round );
    bool keepPassingFrame();

    unsigned int id_;
    Station *next_;
    struct SendRequest {
        unsigned int round;
        unsigned int dst;
        unsigned int prio;
    };
    std::list<SendRequest> sendRequests_;

    static int numPendingRequests;
  public:
    Station( unsigned int id );
    void setup( Station *nexthop );	// supply next hop
    void sendreq( unsigned int round, unsigned int dst, unsigned int prio );
    void start();

}; // Station

#endif // __STATION_H__
