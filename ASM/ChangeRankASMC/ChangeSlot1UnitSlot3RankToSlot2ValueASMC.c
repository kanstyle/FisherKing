#include "ChangeSlot1UnitSlot3RankToSlot2ValueASMC.h" // headers 

void ChangeSlot1UnitSlot3RankToSlot2ValueASMC(ProcPtr proc) {
	u8 unitID = gEventSlots[1];
	u8 target = gEventSlots[2];
	u8 rank = gEventSlots[3];
	struct Unit* curUnit = GetUnitFromCharId(unitID);
	curUnit->ranks[rank] = target;
}