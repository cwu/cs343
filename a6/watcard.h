#ifndef __A6WATCARD_H__
#define __A6WATCARD_H__

#include <uC++.h>
#include <uFuture.h>

class WATCard {
  private:
    unsigned int balance;
    WATCard( const WATCard & ) {}			// prevent copying
    WATCard &operator=( const WATCard & card);
  public:
    WATCard();
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

typedef Future_ISM<WATCard *> FWATCard;		// future WATCard pointer

#endif
