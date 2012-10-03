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

    /**
     * Accepts a frame from another station.
     *
     * frame - the frame that was sent
     */
    void data( Frame frame );				// pass frame

    /**
     * The main coroutine to process frames.
     */
    void main();

    /**
     * Checks if there is still any pending requests held by this station.
     *
     * round - the current round.
     *
     * returns true if there is a pending request, false otherwise.
     */
    bool hasPendingRequest( unsigned int round );

    /**
     * Checks if we should still keep passing frames around. Essentially
     * acts to determine when to terminate.
     *
     * returns true if the frame should be kept passing on. false otherwise.
     */
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
