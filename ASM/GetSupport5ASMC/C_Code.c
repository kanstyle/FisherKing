#include "C_Code.h" // headers 

void GetSlot1UnitSupportFlagInSlot8ASMC(ProcPtr proc) {
	u8 unitID = gEventSlots[1];
	struct Unit* curUnit = GetUnitFromCharId(unitID);
	gEventSlots[0x8] = curUnit->supportBits;
}