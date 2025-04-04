#include "C_Code.h" // headers 
extern void* KillEvent; 

void TryAddUnitToHealTargetList(struct Unit* unit) {

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    if (GetUnitCurrentHp(unit) == GetUnitMaxHp(unit)) {
        return;
    }
	
	if (unit->statusIndex == UNIT_STATUS_PETRIFY) {
		return;
	}

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

bool GetDyingPetrifiedUnit(ProcPtr proc) {
	int i;
	
	for (i = FACTION_BLUE + 1; i < FACTION_BLUE + 0x40; i++)
    {
        struct Unit * unit = GetUnit(i);
		
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        if (unit->statusIndex == 0xB && unit->statusDuration == 1) {
			gEventSlots[0xC] = 1;
			gActiveUnit = unit;
			return true;
		}
	}
	gEventSlots[0xC] = 0;
	gActiveUnit = 0;
	return false;
}

void ExecRestore(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));

    if (GetUnit(gActionData.targetIndex)->statusIndex == UNIT_STATUS_PETRIFY) {
        SetUnitStatus(GetUnit(gActionData.targetIndex), UNIT_STATUS_NONE);
        GetUnit(gActionData.targetIndex)->state &= ~(US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI);
		GetUnit(gActionData.targetIndex)->supportBits &= 0xFE;
    }

    SetUnitStatus(GetUnit(gActionData.targetIndex), UNIT_STATUS_NONE);

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}

void GetSlot7UnitSupportFlagInSlot8ASMC(ProcPtr proc) {
	u8 unitID = gEventSlots[7];
	struct Unit* curUnit = GetUnitFromCharId(unitID);
	gEventSlots[0x8] = curUnit->supportBits;
}

void ReviveAndSetPetrify(ProcPtr proc) {
	//gEventSlots[7] = gActiveUnit->pCharacterData->number;
	GetSlot7UnitSupportFlagInSlot8ASMC(proc);
	if (gEventSlots[8] == 0) { //check support flag
		u8 unitID = gEventSlots[7];
		struct Unit* curUnit = GetUnitFromCharId(unitID);
		
		curUnit->state &= ~(US_HIDDEN|US_DEAD);
		curUnit->curHP = 1;
		curUnit->state |= (US_HAS_MOVED);
		curUnit->statusIndex = 0xB;
		curUnit->statusDuration = 0x3;
		curUnit->supportBits |= 0x1; //set support flag
	}
}

void CullPetrifiedUnits(ProcPtr proc) {
	while (GetDyingPetrifiedUnit(proc) == true) {
		CallEvent(&KillEvent, 1);
	}
}

void PetrifyDeathQuote(ProcPtr proc) {
	if (GetUnit(gEventSlots[1])->statusIndex == 0xB) {
		CallEvent(&KillEvent, 1);
	}
	else {
		ReviveAndSetPetrify(proc);
	}
}