#include "C_Code.h" // headers 

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

void GetDyingPetrifiedUnitASMC(ProcPtr proc) {
	int i;
	gEventSlots[0xC] = 0x0;
	
	for (i = FACTION_BLUE + 1; i < FACTION_BLUE + 0x40; i++)
    {
        struct Unit * unit = GetUnit(i);
		
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        if (unit->statusIndex == 0xB && unit->statusDuration == 1) {
			gActiveUnit = unit;
			gEventSlots[0xC] = 0x1;
			break;
		}
	}
	return;
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