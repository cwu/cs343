#include <iostream>
using namespace std;
#include <cstdlib>                        // exit, atoi

#ifndef NOOUTPUT
struct T {
    ~T() { cout << "~T" << endl; }
};
#endif

struct E {};
unsigned int hc, gc, fc, kc;

void f( volatile int i ) {                // volatile, prevent dead-code optimizations
#ifndef NOOUTPUT
    T t;
    cout << "f enter" << endl;
#endif
  if ( i == 3 ) throw E();
    if ( i != 0 ) f( i - 1 );
#ifndef NOOUTPUT
    cout << "f exit" << endl;
#endif
    kc += 1;                              // prevent tail recursion optimization
}
void g( volatile int i ) {
#ifndef NOOUTPUT
    cout << "g enter" << endl;
#endif
    if ( i % 2 == 0 ) f( fc );
    if ( i != 0 ) g( i - 1 );
#ifndef NOOUTPUT
    cout << "g exit" << endl;
#endif
    kc += 1;
}
void h( volatile int i ) {
#ifndef NOOUTPUT
    cout << "h enter" << endl;
#endif
    if ( i % 3 == 0 ) {
        try {
            f( fc );
        } catch( E ) {
#ifndef NOOUTPUT
            cout << "handler 1" << endl;
#endif
            try {
                g( gc );
            } catch( E ) {
#ifndef NOOUTPUT
                cout << "handler 2" << endl;
#endif
            }
        }
    }
    if ( i != 0 ) h( i - 1 );
#ifndef NOOUTPUT
    cout << "h exit" << endl;
#endif
    kc += 1;
} 

int main( int argc, char *argv[] ) {
    if ( argc != 4 ) {
        cerr << "Usage: " << argv[0] << " hc gc fc" << endl;
        exit( EXIT_FAILURE );
    } // if
    hc = atoi( argv[1] );                 // h recursion depth
    gc = atoi( argv[2] );                 // g recursion depth
    fc = atoi( argv[3] );                 // f recursion depth
    if ( hc < 0 || gc < 0 || fc < 0 ) {
        cerr << "Input less than 0" << endl;
        exit( EXIT_FAILURE );
    } // if

    h( hc );
}
