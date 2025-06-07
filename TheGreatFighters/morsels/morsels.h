
#define TGF_MORSELS_CHARACTER_COUNT 1

#include "TONSOFEFFECTS/TONSOFEFFECTS.h"
#include "STREETFIGHTER/STREETFIGHTER.h"

struct Character* getCharacterByOrder(unsigned int i, struct Dimension* dim, iVector position)
{
    if (i == 0) return tgfGetStreetFighter(dim, position);
}



#include "morsels_core.h"