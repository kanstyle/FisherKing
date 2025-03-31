#include "gbafe.h"

//Skill System's new 8-byte long rounds data.
struct NewBattleHit {
/*00*/  unsigned attributes : 19;
/*02*/  unsigned info       : 5;
/*03*/  s8 damage;
/*04*/  u8 skillID;
/*05*/  s8 hpChange;
/*06*/  s16 totalChange;
};

typedef struct NewBattleHit NewBattleHit;

enum {
	BATTLE_HIT_ATTR_SKIP		= (1 << 5),
	BATTLE_HIT_ATTR_ATKSKILL	= (1 << 14),
	BATTLE_HIT_ATTR_DEFSKILL	= (1 << 15),
};

/*
//From FE-CLib, copied here for reference
enum {
	BATTLE_HIT_ATTR_CRIT      = (1 << 0),
	BATTLE_HIT_ATTR_MISS      = (1 << 1),
	BATTLE_HIT_ATTR_FOLLOWUP  = (1 << 2),
	BATTLE_HIT_ATTR_RETALIATE = (1 << 3),
	BATTLE_HIT_ATTR_BRAVE     = (1 << 4),
	BATTLE_HIT_ATTR_POISON    = (1 << 6),
	BATTLE_HIT_ATTR_DEVIL     = (1 << 7),
	BATTLE_HIT_ATTR_HPSTEAL   = (1 << 8),
	BATTLE_HIT_ATTR_HPHALVE   = (1 << 9),
	BATTLE_HIT_ATTR_TATTACK   = (1 << 10), // triangle attack!
	BATTLE_HIT_ATTR_SILENCER  = (1 << 11),
	BATTLE_HIT_ATTR_12        = (1 << 12), // unused?
	BATTLE_HIT_ATTR_PETRIFY   = (1 << 13),
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
*/
extern NewBattleHit NewBattleHitArray[];
