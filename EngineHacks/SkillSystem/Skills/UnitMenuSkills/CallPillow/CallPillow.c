#include "gbafe.h"
 
 // skill sys 
extern int SkillTester(struct Unit* unit, int id); 
extern int CallPillowID_Link;
extern int PillowID_Link;
extern u16* CallPillowEvent; 

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
 
int CallPillow_Usability(struct MenuProc* menu) { 
	if (SkillTester(gActiveUnit, CallPillowID_Link) == 0) { 
		return 3; // false 
	}
	return 1; //usable 
} 

int CallPillowAction(struct Proc* proc) {	
	
	gEventSlots[7] = gActiveUnit->xPos;
	gEventSlots[8] = gActiveUnit->yPos;
	gEventSlots[5] = GetUnit(PillowID_Link)->xPos;
	gEventSlots[6] = GetUnit(PillowID_Link)->yPos;
	CallEvent(&CallPillowEvent, 1); 

	return (ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX); // parent proc yields 
} 
 
 