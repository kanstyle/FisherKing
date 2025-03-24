#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

int GetUnitExpCalcLevel(struct Unit* unit, bool isTarget){
    int result = unit->level;
    result += (2 * GetClassData(unit->pClassData->promotion)->classRelativePower);
    if(UNIT_CATTRIBUTES(unit) & CA_PROMOTED){
        if(isTarget){
            result += 20 ;
        } else {
            result += 18;
        }
    }
    return result;
}

int GetBossExpBonus(int levelDiff){
    int bossExpBonus = 13 - (7 * (levelDiff - 3) / 2);
    if(bossExpBonus > 30){
        bossExpBonus = 30;
    } 
    if(bossExpBonus < 4){
        bossExpBonus = 4;
    }
    return bossExpBonus;
}


int GetUnitRoundExp(struct Unit* actor, struct Unit* target) {
    int levelDiff = GetUnitExpCalcLevel(actor, false) - GetUnitExpCalcLevel(target, true);
    int result;
    bool atOverLevel = levelDiff >= 0;
    if(target->curHP == 0){ // is kill
        if(atOverLevel){
            result = 30 - ((9 * levelDiff) / 2);
            if(result < 1){
                result = 1;
            }
        } else {
            result = 30 + ((1 + levelDiff * -10) / 3); // negative bc leveldiff is negative so double negation
        }

        if(UNIT_CATTRIBUTES(target) & CA_BOSS){
            result += GetBossExpBonus(levelDiff);
        }
    } else { // is not kill
        result = atOverLevel? 10 - (( 5 * levelDiff ) / 3) : 10 - (levelDiff / 2); // negative bc leveldiff is negative so double negation
    }
    return result;
}

int GetBattleUnitExpGain(struct BattleUnit* actor, struct BattleUnit* target) {
    int result;

    if (!CanBattleUnitGainLevels(actor) || (actor->unit.curHP == 0) || UNIT_CATTRIBUTES(&target->unit) & CA_NEGATE_LETHALITY)
        return 0;

    result = GetUnitRoundExp(&actor->unit, &target->unit);

    if (!actor->nonZeroDamage && result > 0)
        return 1;

    if (result > 100)
        result = 100;

    if (result < 0)
        result = 0;

    return result;
}
