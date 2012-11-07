#include <uC++.h>

#if defined( CLASS )
class
#elif defined( COROUTINE )
_Coroutine
#elif defined( TASK )
_Task
#endif
CallCxtSw {
	unsigned int times;
	volatile unsigned int i;		// ignore, force call to rtn

	void rtn();
	void main();
  public:
	CallCxtSw( int times ) : times( times ), i( 0 ) {
#if defined( CLASS )
		main();
#elif defined( COROUTINE )
		resume();
#endif
	}
};

void CallCxtSw::main() {
	for ( int i = 1; i <= times; i += 1 ) {
#if defined( CLASS )
		rtn();
#elif defined( COROUTINE )
		resume();
#elif defined( TASK )
		yield();
#endif
	} // for
}
void CallCxtSw::rtn() { i += 1; }

void uMain::main() {
	unsigned int times = 10000000;
	switch ( argc ) {
	  case 2:
		times = atoi( argv[1] );
	} // switch

	CallCxtSw obj( times );
}
