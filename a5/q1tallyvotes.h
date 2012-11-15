#ifndef Q1TALLYVOTES_H
#define Q1TALLYVOTES_H

#include <uC++.h>

class Printer;

#if defined( IMPLTYPE_LOCK )		// mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
 	// private declarations for this kind of vote-tallier

#elif defined( IMPLTYPE_EXT )		// external scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
	// private declarations for this kind of vote-tallier

#elif defined( IMPLTYPE_INT )		// internal scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
	// private declarations for this kind of vote-tallier

#elif defined( IMPLTYPE_AUTO )		// automatic-signal monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
	// private declarations for this kind of vote-tallier

#elif defined( IMPLTYPE_TASK )		// internal/external scheduling task solution
_Task TallyVotes {
	// private declarations for this kind of vote-tallier

#else
#error unsupported voter type
#endif

	// common declarations
    unsigned int group;
    Printer &printer;
  public:
	// common interface
	TallyVotes( unsigned int group, Printer &printer );
	bool vote( unsigned int id, bool ballot );
};

#endif // Q1TALLYVOTES_H
