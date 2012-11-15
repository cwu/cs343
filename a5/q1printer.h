#ifndef Q1PRINTER_H
#define Q1PRINTER_H

#include <uC++.h>

#include "q1voter.h"

_Monitor Printer { // chose one of the two kinds of type constructor
  public:
	Printer( unsigned int voters );
	void print( unsigned int id, Voter::States state );
	void print( unsigned int id, Voter::States state, bool vote );
	void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif // Q1PRINTER_H
