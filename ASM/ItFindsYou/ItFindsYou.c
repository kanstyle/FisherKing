#include "gbafe.h"
#include "ItFindsYou.h"
 
 // skill sys 
extern int SkillTester(struct Unit* unit, int id); 
extern int ItFindsYouID_Link;

void ItFindsYouEffect(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	if (SkillTester(bunitA, ItFindsYouID_Link) == 0) {
		return;
	}
	else {
		if (bunitA->battleEffectiveHitRate >= 70) {
			bunitA->battleEffectiveHitRate = 100;
		}
	}
	return;
}