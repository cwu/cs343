#ifndef AUTOMATICSIGNAL_H
#define AUTOMATICSIGNAL_H

#include <uC++.h>

#define AUTOMATIC_SIGNAL \
    uCondition waitPred
#define WAITUNTIL(pred, before, after) \
    if (!(pred)) { \
        before; \
        while (!waitPred.empty()) { waitPred.signal(); } \
        while (!(pred)) { \
            waitPred.wait(); \
        } \
        after; \
    }
#define RETURN(expr...) \
    while (!waitPred.empty()) { waitPred.signal(); } \
    return (expr)

#endif // AUTOMATICSIGNAL_H

