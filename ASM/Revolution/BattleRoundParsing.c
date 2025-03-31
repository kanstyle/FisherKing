#include "BattleRoundParsing.h"

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

//NOTE: this function may not work as expected on the first iteration of the BattleProcCalcLoop
NewBattleHit* getFirstDefenderRound(NewBattleHit* round) {
	//Iterate through rounds until we find one marked as last
	while(true) {
		if (isDefenderRound(round)) return round;
		if (isLastRound(round)) return 0;
		round++;
	}
}

bool didAttackerLandHit() {
	for (int i = 0; ; i++) {
		if (!isDefenderRound(&NewBattleHitArray[i]) && roundHits(&NewBattleHitArray[i])) return true;
		if (isLastRound(&NewBattleHitArray[i])) return false;
	}
}

bool didDefenderLandHit() {
	for (int i = 0; ; i++) {
		if (isDefenderRound(&NewBattleHitArray[i]) && roundHits(&NewBattleHitArray[i])) return true;
		if (isLastRound(&NewBattleHitArray[i])) return false;
	}	
}
