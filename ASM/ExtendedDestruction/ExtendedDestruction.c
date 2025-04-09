#include "gbafe.h"
#include "ExtendedDestruction.h"
 
 // skill sys 
extern int SkillTester(struct BattleUnit* unit, int id); 
//extern int SkillTester(struct Unit* unit, int id); 
extern int ExtendedDestructionID_Link;
extern int SpecialStatusID_Link;
extern void* ExtendedDestructionEvent;

enum MenuEffect {
    //
    ME_NONE = 0,

    ME_DISABLE = (1 << 0),
    ME_END = (1 << 1),
    ME_PLAY_BEEP = (1 << 2),
    ME_PLAY_BOOP = (1 << 3),
    ME_CLEAR_GFX = (1 << 4),
    ME_END_FACE0 = (1 << 5),
    ME_END_AFTER = (1 << 7),
};
 
int ExtendedDestruction_Usability(struct MenuProc* menu) { 
	if (gActiveUnit->statusIndex == SpecialStatusID_Link) { 
		return 3; // false if you already have the status
	} 
	if (SkillTester(gActiveUnit, ExtendedDestructionID_Link) == 0) { 
		return 3; // false 
	}
	return 1; //usable 
} 

int ExtendedDestructionAction(struct Proc* proc) {	
	
	gActiveUnit->statusIndex = SpecialStatusID_Link;
	gActiveUnit->statusDuration = 9;
	gActionData.unitActionType = UNIT_ACTION_WAIT;
	
	CallEvent(&ExtendedDestructionEvent, 1); 

	return (ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX); // parent proc yields 
} 

u8 ExtendedDestructionMovBuff(u8 stat, struct Unit* unit) {
	if (unit->statusIndex & SpecialStatusID_Link) {
		if (SkillTester(unit, ExtendedDestructionID_Link) != 0) {
			stat += 2;
		}
	}
	return stat;
}

void ExtendedDestructionAtkBuff(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	if (bunitA->unit.statusIndex & SpecialStatusID_Link) {
		if (SkillTester(bunitA, ExtendedDestructionID_Link) != 0) {
			bunitA->battleAttack += 5;
		}
	}
	return;
}

void ExecStandardHeal(ProcPtr proc) {
    int amount;

    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));

    amount = GetUnitItemHealAmount(
        GetUnit(gActionData.subjectIndex),
        GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex]
    );

    AddUnitHp(GetUnit(gActionData.targetIndex), amount);

    gBattleHitIterator->hpChange = gBattleTarget.unit.curHP - GetUnitCurrentHp(GetUnit(gActionData.targetIndex));

    gBattleTarget.unit.curHP = GetUnitCurrentHp(GetUnit(gActionData.targetIndex));

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();
	
	if (GetUnit(gActionData.targetIndex)->statusIndex & SpecialStatusID_Link) {
		if (SkillTester(GetUnit(gActionData.targetIndex), ExtendedDestructionID_Link) != 0) {
			GetUnit(gActionData.targetIndex)->statusIndex = 0;
			GetUnit(gActionData.targetIndex)->statusDuration = 0;
		}
	}

    return;
}