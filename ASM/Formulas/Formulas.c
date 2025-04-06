#include "gbafe.h"
#include "Formulas.h"
 
 // skill sys 
//extern int SkillTester(struct Unit* unit, int id); 

void ComputeBattleUnitCritRate(struct BattleUnit* bu) {
    bu->battleCritRate = GetItemCrit(bu->weapon);

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_CRITBONUS)
        bu->battleCritRate += 15;
}

void ComputeBattleUnitDodgeRate(struct BattleUnit* bu) {
    bu->battleDodgeRate = 0;
}