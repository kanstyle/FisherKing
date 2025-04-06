#include "gbafe.h"
#include "Revolution.h"
 
 // skill sys 
extern int SkillTester(struct Unit* unit, int id); 
extern int RevolutionID_Link;

/*enum MenuEffect {
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

enum {
    BATTLE_HIT_ATTR_CRIT      = (1 << 0),
    BATTLE_HIT_ATTR_MISS      = (1 << 1),
    BATTLE_HIT_ATTR_FOLLOWUP  = (1 << 2),
    BATTLE_HIT_ATTR_RETALIATE = (1 << 3),
    BATTLE_HIT_ATTR_BRAVE     = (1 << 4),
    BATTLE_HIT_ATTR_5         = (1 << 5), // unused?
    BATTLE_HIT_ATTR_POISON    = (1 << 6),
    BATTLE_HIT_ATTR_DEVIL     = (1 << 7),
    BATTLE_HIT_ATTR_HPSTEAL   = (1 << 8),
    BATTLE_HIT_ATTR_HPHALVE   = (1 << 9),
    BATTLE_HIT_ATTR_TATTACK   = (1 << 10), // triangle attack!
    BATTLE_HIT_ATTR_SILENCER  = (1 << 11),
    BATTLE_HIT_ATTR_12        = (1 << 12), // unused?
    BATTLE_HIT_ATTR_PETRIFY   = (1 << 13),
    BATTLE_HIT_ATTR_SURESHOT  = (1 << 14),
    BATTLE_HIT_ATTR_GREATSHLD = (1 << 15),
    BATTLE_HIT_ATTR_PIERCE    = (1 << 16),
    BATTLE_HIT_ATTR_17        = (1 << 17), // unused?
    BATTLE_HIT_ATTR_18        = (1 << 18), // unused?
};

enum {
    BATTLE_HIT_INFO_BEGIN        = (1 << 0),
    BATTLE_HIT_INFO_FINISHES     = (1 << 1),
    BATTLE_HIT_INFO_KILLS_TARGET = (1 << 2),
    BATTLE_HIT_INFO_RETALIATION  = (1 << 3),
    BATTLE_HIT_INFO_END          = (1 << 4),
};

enum
{
    // Unit state constant masks

    US_NONE         = 0,

    US_HIDDEN       = (1 << 0), //1
    US_UNSELECTABLE = (1 << 1), //2
    US_DEAD         = (1 << 2), //4
    US_NOT_DEPLOYED = (1 << 3), //8
    US_RESCUING     = (1 << 4), //16
    US_RESCUED      = (1 << 5), //32
    US_HAS_MOVED    = (1 << 6), // Bad name?
    US_CANTOING     = US_HAS_MOVED, // Alias
    US_UNDER_A_ROOF = (1 << 7),
    US_BIT8 = (1 << 8), // has been seen?
    US_BIT9 = (1 << 9), // hidden by fog?
    US_HAS_MOVED_AI = (1 << 10),
    US_IN_BALLISTA  = (1 << 11),
    US_DROP_ITEM    = (1 << 12),
    US_GROWTH_BOOST = (1 << 13),
    US_SOLOANIM_1   = (1 << 14),
    US_SOLOANIM_2   = (1 << 15),
    US_BIT16        = (1 << 16),
    US_BIT17        = (1 << 17),
    US_BIT18        = (1 << 18),
    US_BIT19        = (1 << 19),
    US_BIT20        = (1 << 20),
    US_BIT21        = (1 << 21),
    US_BIT22        = (1 << 22),
    US_BIT23        = (1 << 23),
    // = (1 << 24),
    US_BIT25 = (1 << 25),
    US_BIT26 = (1 << 26),
    US_BIT27 = (1 << 27),
    // = (1 << 28),
    // = (1 << 29),
    // = (1 << 30),
    // = (1 << 31),

    // Helpers
    US_UNAVAILABLE = (US_DEAD | US_NOT_DEPLOYED | US_BIT16),
    US_SOLOANIM = (US_SOLOANIM_1 | US_SOLOANIM_2),
};*/
/*
void BattlePrintDebugHitInfo(void) {
    struct BattleHit* it;

    for (it = gBattleHitArray; !(it->info & BATTLE_HIT_INFO_END); ++it) {
        // prints battle rounds information to debug output
    }
}
*/
bool isFirstRound(NewBattleHit* round) {
	return (round->info & BATTLE_HIT_INFO_BEGIN);
}

bool isLastRound(NewBattleHit* round) {
	return (round->info & BATTLE_HIT_INFO_END);
}

bool isDefenderRound(NewBattleHit* round) {
	return (round->info & BATTLE_HIT_INFO_RETALIATION);
}

bool roundHits(NewBattleHit* round) {
	return !(round->attributes & BATTLE_HIT_ATTR_MISS);
}

bool roundCrits(NewBattleHit* round) {
	return (round->attributes & BATTLE_HIT_ATTR_CRIT);
}

bool isFollowupAttack(NewBattleHit* round) {
	return (round->attributes & BATTLE_HIT_ATTR_FOLLOWUP);
}

bool killsTarget(NewBattleHit* round) {
	return (round->info & BATTLE_HIT_INFO_KILLS_TARGET);
}

int getDamage(NewBattleHit* round) {
	return (round->damage);
}

int getTotalChange(NewBattleHit* round) {
	return (round->totalChange);
}

bool CheckOHKO(struct Proc* proc) {	
	
	struct NewBattleHit* it;

    for (it = NewBattleHitArray; !(it->info & BATTLE_HIT_INFO_END); ++it) {
		gEventSlots[2] = getTotalChange(it);
		gEventSlots[3] = abs(getDamage(it));
		gEventSlots[4] = gBattleTarget.unit.maxHP;
		gEventSlots[5] += 0x1;
        if (isDefenderRound(it)) { //if defender, break
			gEventSlots[6] = 0x1;
			break;
		}
		if (!roundHits(it)) { //if attack missed, break
			gEventSlots[6] = 0x2;
			break;
		}
		if (gBattleTarget.unit.maxHP > abs(getDamage(it))) { //if hit dealt less damage than target's max HP, break

			gEventSlots[6] = 0x3;
			break;
		}
		if (killsTarget(it)) { //if attacker, and not a miss, and end of battle
			gEventSlots[6] = 0x4;
			return true;
		}
    }
	return false;
} 

void DoGaleforce(struct Proc* proc) {
	if (!(gActiveUnit->state & 0x400)) { //check galeforce bit
		gActiveUnit->state &= ~0x40; //unset has_moved
		gActiveUnit->state &= ~0x2; //unset unselectable
		gActiveUnit->state |= 0x400; //set galeforce bit
	}
}

void RevolutionEffect(struct Proc* proc) {
	if (SkillTester(gActiveUnit, RevolutionID_Link) == 0) {
		gEventSlots[6] = 0x5;
		return;
	}
	if (CheckOHKO(proc) == true) {
		DoGaleforce(proc);
	}
}

/*void GetUnitCoords(u8 unitID, u8 slot, struct Proc* proc) {
	gEventSlots[7] = GetUnit(unitID)->xPos;
	gEventSlots[8] = GetUnit(unitID)->yPos;
	
	gEventSlots[slot] = gEventSlots[7] << 16;
	gEventSlots[slot] += gEventSlots[8];
}*/
 
 bool IsThisFirstCrit(struct Proc* proc) {
	struct NewBattleHit* it;

    for (it = NewBattleHitArray; !(it->info & BATTLE_HIT_INFO_END); ++it) {
		
	}
 }