#include <cstdlib>			// atoi

void rtn( volatile int i ) {		// ignore volatile, prevents eliminating increment of i
	if ( i == 0 ) throw 0;		// recursion base case
	rtn( i - 1 );
	i += 1;				// ignore, needed to prevent tail-recursion optimization
}

int main( int argc, char *argv[] ) {
	int times = 100000, recurse = -1;
	switch ( argc ) {
	  case 3:
		 recurse = atoi( argv[2] );
	  case 2:
		 times = atoi( argv[1] );
	} // switch

	for ( int t = 0; t < times; t += 1 ) {
#ifdef EXCEPT
		try {
#endif
			for ( int i = 0; i < 10; i += 1 ) {
				for ( int j = 0; j < 10; j += 1 ) {
					for ( int k = 0; k < 10; k += 1 ) {
						if ( argc >= 1 || ( i == 5 && j == 3 && k == 5 ) ) { // ignore, prevent loop elimination
#ifdef EXCEPT
							if ( recurse < 0 ) throw 0;	// administrative cost, no stack unwinding
							else rtn( recurse );		// try 2,4,8 for cost of stack unwinding
#else
							goto fini;
#endif
						} // if
					} // for
				} // for
			} // for
		  fini: ;
#ifdef EXCEPT
		} catch( int ) {
		} // try
#endif
	} // for
} // main
