#include "C_Code.h" // headers 

//#define VANILLA_VERSION 
static int sSupportMaxExpLookup[] = {
    [SUPPORT_LEVEL_NONE] = SUPPORT_EXP_C-1,
    [SUPPORT_LEVEL_C]    = SUPPORT_EXP_B-1,
    [SUPPORT_LEVEL_B]    = SUPPORT_EXP_A-1,
    [SUPPORT_LEVEL_A]    = SUPPORT_EXP_A
};

s8 CanUnitSupportNow(struct Unit* unit, int num)
{
    int exp, maxExp;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return FALSE;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_TUTORIAL)
        return FALSE;

    if (HasUnitGainedSupportLevel(unit, num))
        return FALSE;

    if (GetUnitTotalSupportLevel(unit) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (GetUnitTotalSupportLevel(GetUnitSupporterUnit(unit, num)) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    exp    = unit->supports[num];
    maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

    if (exp == SUPPORT_EXP_A)
        return FALSE;

    return (exp == maxExp) ? TRUE : FALSE;
}

int sub_80A0F6C(s8 flag, int idx) {
    int i;
    int a;
    int b;
    int c;

    if (flag != 0) {
        int var = GetTotalSupportLevel(idx);

        if (var == MAX_SIMULTANEOUS_SUPPORT_COUNT) {
            return 2;
        }

        if (var == 0) {
            return 0;
        }

        return 1;
    }

    a = 0;
    b = GetTotalSupportLevel(idx);

    c = GetSupportScreenPartnerCount(GetSupportScreenCharIdAt(idx));

    for (i = 0; i < c; i++) {
        a += GetUnitsAverageSupportValue(GetSupportScreenCharIdAt(idx), GetSupportScreenPartnerCharId(idx, i));
    }

    if (a == b) {
        return 2;
    }

    if (b == 0) {
        return 0;
    }

    return 1;
}

void DisplaySupportList(void)
{
    int yTile = 6, lineNum = 0;

    int textColor = GetUnitTotalSupportLevel(gStatScreen.unit) == MAX_SIMULTANEOUS_SUPPORT_COUNT
        ? TEXT_COLOR_SYSTEM_GREEN
        : TEXT_COLOR_SYSTEM_WHITE;

    int supportAmt = GetUnitSupporterCount(gStatScreen.unit);
    int supportId  = 0;

    while (supportId < supportAmt)
    {
        int level = GetUnitSupportLevel(gStatScreen.unit, supportId);

        if (level != 0)
        {
            int rankColor;

            u8 pid = GetUnitSupporterCharacter(gStatScreen.unit, supportId);

            DrawIcon(
                gUiTmScratchA + TILEMAP_INDEX(4, yTile),
                GetCharacterAffinityIcon(pid),
                TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

            PutDrawText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT0 + lineNum],
                gUiTmScratchA + TILEMAP_INDEX(7, yTile),
                textColor, 0, 0,
                GetStringFromIndex(GetCharacterData(pid)->nameTextId));

            rankColor = TEXT_COLOR_SYSTEM_BLUE;

            if (level == 3)
                rankColor = TEXT_COLOR_SYSTEM_GREEN;

            if (textColor == TEXT_COLOR_SYSTEM_GREEN)
                rankColor = TEXT_COLOR_SYSTEM_GREEN;

            PutSpecialChar(gUiTmScratchA + TILEMAP_INDEX(13, yTile),
                rankColor, GetSupportLevelUiChar(level));

            yTile += 2;
            lineNum++;
        }

        supportId++;
    }
}