#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <uC++.h>

_Coroutine Grammar {
  public:
    enum Status { CONT, MATCH, ERROR };  // send more characters, in language, invalid
  private:
    Status status;                    // current status of match
    char ch;                          // character passed by cocaller

    void main();
  public:
    Status next( char c ) {
        ch = c;                        // communication in
        resume();                      // activate
        return status;                 // communication out
    } // Grammar::next
}; // Grammar

#endif // __GRAMMAR_H__
