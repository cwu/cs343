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

  public:
    Station( unsigned int id );
    void setup( Station *nexthop );	// supply next hop
    void sendreq( unsigned int round, unsigned int dst, unsigned int prio );
    void start();

}; // Station
