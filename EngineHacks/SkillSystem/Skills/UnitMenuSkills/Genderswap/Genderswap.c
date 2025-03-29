#include "gbafe.h"
 
 // skill sys 
extern int SkillTester(struct Unit* unit, int id); 
extern int GenderswapID_Link;
extern int GenderswapFlag_Link;
extern int UpstartM_Link;
extern int UpstartF_Link;
extern u16* GenderswapEvent; 

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
 
int Genderswap_Usability(struct MenuProc* menu) { 
	if (CheckFlag(GenderswapFlag_Link) == 1) { //if switched genders this turn already 
		return 3; // false 
	} 
	if (gActiveUnit->state & US_CANTOING) { 
		return 3; // false 
	} 
	if (SkillTester(gActiveUnit, GenderswapID_Link) == 0) { 
		return 3; // false 
	}
	return 1; //usable 
} 

int GenderswapAction(struct Proc* proc) {	
	SetFlag(GenderswapFlag_Link); //HeroGenderswapFlag 
	
	//male to female
	if (gActiveUnit->pClassData->number == UpstartM_Link) {		
		int bowRank = gActiveUnit->ranks[3];
		int staffRank = gActiveUnit->ranks[4];
		
		gActiveUnit->ranks[0] = bowRank; //bow turns into sword
		gActiveUnit->ranks[6] = staffRank; //staff turns into bomb
		gActiveUnit->ranks[3] = 0; //clear bow rank
		gActiveUnit->ranks[4] = 0; //clear staff rank
		
		gActiveUnit->pClassData = GetClassData(UpstartF_Link);
	}
	
	//female to male
	else if (gActiveUnit->pClassData->number == UpstartF_Link) {		
		int swordRank = gActiveUnit->ranks[0];
		int bombRank = gActiveUnit->ranks[6];
		
		gActiveUnit->ranks[3] = swordRank; //sword turns into bow
		gActiveUnit->ranks[4] = bombRank; //bomb turns into staff
		gActiveUnit->ranks[0] = 0; //clear sword rank
		gActiveUnit->ranks[6] = 0; //clear bomb rank
		
		gActiveUnit->pClassData = GetClassData(UpstartM_Link);
	}
	
	int actorHP = gActiveUnit->curHP;
	int amountToHeal = gActiveUnit->maxHP/4; 
	int actorMax = gActiveUnit->maxHP;
	if ((amountToHeal + actorHP) > actorMax) { 
		amountToHeal = actorMax - actorHP; 
	} 

	//gActiveUnit->curHP -= amountToHeal; // if not showing anim
	gEventSlots[1] = gActiveUnit->pCharacterData->number; //unit ID
	gEventSlots[6] = amountToHeal; //heal amt
	//gActiveUnit->state = gActiveUnit->state & ~US_HIDDEN; 
	//SMS_UpdateFromGameData();
	//MU_EndAll();
	CallEvent(&GenderswapEvent, 1); 

	return (ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX); // parent proc yields 
} 
 
 