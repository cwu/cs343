#include "q1tallyvotes.h"


TallyVotes::TallyVotes( unsigned int group, Printer &printer )
    : group(group), printer(printer) {
}
bool TallyVotes::vote( unsigned int id, bool ballot ) {
    return false;
}
