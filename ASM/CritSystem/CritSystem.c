#include "gbafe.h"
#include "CritSystem.h"

extern int ValueToCrit_Link;
extern int CritDamageMult_Link;

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

bool IsFirstCrit(struct BattleUnit* attacker) {
	struct NewBattleHit* it;

    for (it = NewBattleHitArray; !(it->info & BATTLE_HIT_INFO_END); ++it) {
		gEventSlots[5] += 0x1;
		if (attacker == &gBattleActor) {
			if (isDefenderRound(it) == false) {
			gEventSlots[2] += 0x1;//if defender, break
				if (roundCrits(it) == false) {
					gEventSlots[3] += 0x1;
					return false;
				}
			}
		}
		else { //if they aren't gBattleActor
			if (isDefenderRound(it) == true) {
			gEventSlots[2] += 0x1;//if defender, break
				if (roundCrits(it) == false) {
					gEventSlots[3] += 0x1;
					return false;
				}
			}
		}
    }
	return true;
}

void BattleGenerateHitAttributes(struct BattleUnit* attacker, struct BattleUnit* defender) {
    short attack, defense;

    gBattleStats.damage = 0;

    BattleCheckSureShot(attacker);

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SURESHOT)) {
        if (!BattleRoll2RN(gBattleStats.hitRate, TRUE)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
            return;
        }
    }

    attack = gBattleStats.attack;
    defense = gBattleStats.defense;

    BattleCheckGreatShield(attacker, defender);

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD))
        BattleCheckPierce(attacker, defender);

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_PIERCE)
        defense = 0;

    gBattleStats.damage = attack - defense;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD)
        gBattleStats.damage = 0;

    if ((gBattleStats.critRate >= ValueToCrit_Link) && (IsFirstCrit(attacker))) {
        if (BattleCheckSilencer(attacker, defender)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;

            gBattleStats.damage = BATTLE_MAX_DAMAGE;

            gBattleHitIterator->attributes = gBattleHitIterator->attributes &~ BATTLE_HIT_ATTR_GREATSHLD;
        } else {
            gBattleHitIterator->attributes = gBattleHitIterator->attributes | BATTLE_HIT_ATTR_CRIT;
            gBattleStats.damage = gBattleStats.damage * CritDamageMult_Link;
        }
    }

    if (gBattleStats.damage > BATTLE_MAX_DAMAGE)
        gBattleStats.damage = BATTLE_MAX_DAMAGE;

    if (gBattleStats.damage < 0)
        gBattleStats.damage = 0;

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}



/*bool CheckOHKO(struct Proc* proc) {	
	
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
}*/