#include "gbafe.h"

/*u8 SupportSixGaleforceMove(u8 stat, struct Unit* unit) {
	if (unit->state & 400) {
		stat = unit->supports[7];
		return stat;
	}
	return stat;
}*/

void ResetSupportSix(struct Proc* proc) {
	int i;
	
	for (i = FACTION_BLUE + 1; i < FACTION_BLUE + 0x40; i++)
    {
        struct Unit * unit = GetUnit(i);
		
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        if (unit->supports[6] > 0) {
			unit->supports[6] = 0;
		}
	}
	return;
}