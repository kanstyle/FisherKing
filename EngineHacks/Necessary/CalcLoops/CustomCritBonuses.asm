.thumb

.macro blh to, reg
  ldr \reg, =\to
  mov lr, \reg
 .short 0xf800
.endm

.set AdjacencyBonus, 			10//bonus per EACH adjacent ally

.set InitiatorBonus,			10//bonus if unit's allegiance matches their phase (player unit attacks in player phase for example)
.set DoOnlyPlayersGetInitator, 	0//If 0 everyone can get it, if 1, only players

.set TriangleBonus,				10//bonus if unit is at wpn triangle advantage

.set DoublingBonus,     		10//bonus if unit can double enemy

.set AttackerBattleStruct,	0x0203A4EC
.set DefenderBattleStruct,	0x0203A56C
.set PlayerCharstruct,		0x0202BE4C
.set EnemyCharstruct,		0x0202CFBC
.set NPCCharstruct,			0x0202DDCC
.set NPCCharstructEnd,		0x0202E36C
.set ChapterDataStruct,     0x0202BCF0
.set PreBattleNumbersData,  0x0203A4D4

.set ValueToCritical,		OutsiderValues

//input:  r0 has unit struct
//output: r0 returns updated crit, r1 is 0 if unit can't crit, 1 if it can
//This technically isn't a function since it returns 2 values,
//but this makes it far more conventient to use
//r0 will hold unit's crit
//r1 will hold unit struct
push	{r4-r7,r14}
	mov     r2,#0x66
	ldrh    r2,[r0,r2]
	mov     r1,r0
	mov     r0,r2
	//now we move to the code that handles bonuses
	mov     r4,r0
	mov     r5,r1
	//all functions need input r0 for crit, r1 for unit's struct
	//all functions need output updated crit in r0
	mov     r0,r4
	mov     r1,r5
	bl      AdjacencyCheck
	mov     r4,r0
	
	mov     r0,r4
	mov     r1,r5
	bl      InitiatorCheck
	mov     r4,r0
	
	mov     r0,r4
	mov     r1,r5
	bl      TriangleCheck
	mov     r4,r0
	
	mov     r0,r4
	mov     r1,r5
	bl      DoublingCheck
	mov     r4,r0	

	mov     r0,r4
	mov     r1,r5
	mov     r2,#0x66
	strh    r0,[r1,r2]
	ldr     r1,#ValueToCritical
	cmp     r0,r1
	bge     CanCritical
	b       CantCritical
	
	CantCritical:
	mov     r1,#0x00
	b       EndCritBonuses

	CanCritical:
	mov     r1,#0x01
	b       EndCritBonuses

	EndCritBonuses:
	mov     r0,r4
pop		{r4-r7}
pop		{r2}
bx      r2

AdjacencyCheck:
//we check for adjacent allies, adding AdjacencyBonus PER EACH adjacent ally that's not rescued, dead or hidden
push	{r4-r7,r14}
	mov     r4,r0
	mov     r5,r1
	mov     r0,r5
	bl      IsUnitStructBattleStruct
	cmp     r0,#0x01
	bne     AdjacencyCheckEnd
	//Adjacency check start
	ldr     r1,=#AttackerBattleStruct
	cmp     r5,r1
	bne     AdjacencyCheckStart//if attacker is not the unit we're checking, we center on defender
	ldr     r0,=#DefenderBattleStruct
	mov     r5,r0
	mov     r1,r5
	b       AdjacencyCheckStart
	AdjacencyCheckStart:
	//r4 has crit, r5 has unit struct
	//First we check if unit is enemy, only enemies can be flanked, and only by players
	ldrb    r0,[r5,#0x0B]
	mov     r1,#0x80
	and     r0,r1
	cmp     r0,r1
	bne     AdjacencyCheckEnd//we're not checking for an enemy, can't be flanked
	//we get defender's coords
	ldrb    r0,[r5,#0x10]
	mov     r6,r0
	ldrb    r0,[r5,#0x11]
	mov     r7,r0
	//r6 has X, r7 has Y
	b       PlayerAdjacency

//	//We get attacker unit's allegiance
//	ldrb    r0,[r5,#0x0B]
//	mov     r1,#0xC0
//	and     r0,r1
//	cmp     r0,#0x00
//	b       PlayerAdjacency
//	beq     PlayerAdjacency
//	cmp     r0,#0x40
//	beq     NPCAdjacency
//	b       EnemyAdjacency
	
	PlayerAdjacency:
	ldr     r0,=#PlayerCharstruct
	ldr     r1,=#EnemyCharstruct
	b       StartAdjacencyCheck

//	EnemyAdjacency:
//	ldr     r0,=#EnemyCharstruct
//	ldr     r1,=#NPCCharstruct
//	b       StartAdjacencyCheck
//	NPCAdjacency:
//	ldr     r0,=#NPCCharstruct
//	ldr     r1,=#NPCCharstructEnd
//	b       StartAdjacencyCheck
	
	StartAdjacencyCheck:
	//first, if attacker is striking from 1 range, we subtract
	//adjacency bonus once, effectively removing them from giving
	//a bonus to crit
	ldr     r2,=#AttackerBattleStruct
	CheckDefenderForAdjacency:
	ldrb    r2,[r2,#0x0B]
	mov     r3,#0xC0
	and     r2,r3
	cmp     r2,#0x00
	beq     AdjacencyCheckNotCountAdjacentAttacker
	ldr     r3,=#DefenderBattleStruct
	cmp     r2,r3
	beq     AdjacencyCheckEnd//Neither unit is a player unit, don't apply bonuses
	mov     r2,r3
	b       CheckDefenderForAdjacency
	
	AdjacencyCheckNotCountAdjacentAttacker:
	//we check if battle is 1 range
	ldr     r2,=#PreBattleNumbersData
	ldrh    r2,[r2,#0x02]
	cmp     r2,#0x01
	bhi     AdjacencyCheckLoop//if range is greater, don't discount
	sub     r4,#AdjacencyBonus//adjacent battle, discount
	
	AdjacencyCheckLoop:
	cmp 	r0,r1
	beq     AdjacencyCheckEnd
	ldr     r2,[r0]
	cmp     r2,#0x00
	beq     AdjacencyCheckNext//empty, unit doesn't exist
	ldrb    r2,[r0,#0x0C]
	mov     r3,#0xA4//dead, being rescued, covered by roof bits
	and     r2,r3
	cmp     r2,#0x00
	bne     AdjacencyCheckNext//unit isn't valid
	//now we get unit coordinates
	ldrb    r2,[r0,#0x10]
	ldrb    r3,[r0,#0x11]
	//r2,r3 has ally X and Y coords, r6,r7 has attacker X and Y coords
	cmp		r2,r6
	beq     CheckY_EqualX
	cmp     r3,r7
	beq     CheckX_EqualY
	b       AdjacencyCheckNext//if neither are equal, unit can't possibly be adjacent
	
	CheckY_EqualX:
	cmp     r3,r7
	beq     AdjacencyCheckNext//If both are equal,attacker and ally are the same unit!
	sub     r3,#0x01
	cmp     r3,r7
	beq     AdjacentTrue
	add     r3,#0x02
	cmp     r3,r7
	beq     AdjacentTrue
	b       AdjacencyCheckNext//if not on -1 or +1 tiles Y when X is equal, not adjacent
	
	CheckX_EqualY:
	cmp     r2,r6
	beq     AdjacencyCheckNext//This should never trigger since we check X first, but oh well
	sub     r2,#0x01
	cmp     r2,r6
	beq     AdjacentTrue
	add     r2,#0x02
	cmp     r2,r6
	beq     AdjacentTrue
	b       AdjacencyCheckNext//if not on -1 or +1 tiles X when Y is equal, not adjacent
	
	AdjacentTrue:
	add     r4,#AdjacencyBonus
	AdjacencyCheckNext:
	add   	r0,#0x48
	b		AdjacencyCheckLoop
	
	AdjacencyCheckEnd:
	mov     r0,r4//we just return crit
pop		{r4-r7}
pop		{r1}
bx      r1

InitiatorCheck:
//we get if attacker's allegiance matches the phase, if so we add InitiatorBonus
push	{r4-r7,r14}
	mov     r4,r0
	mov     r5,r1
	//r4 has crit, r5 has unit struct
	ldr     r0,=#ChapterDataStruct
	ldrb    r0,[r0,#0x0F]//Phase
	mov     r2,#0xC0
	and     r0,r2
	ldrb    r1,[r5,#0x0B]//unit deployment byte
	and     r1,r2
	cmp     r1,r0
	beq     AddInitiatorBonus
	b       EndInitiatorCheck

	AddInitiatorBonus:
	mov     r3,#DoOnlyPlayersGetInitator
	cmp     r3,#0x00
	beq     SkipInitiatorException
	cmp     r1,#0x00
	bne     EndInitiatorCheck//if not 0, unit isn't player
	SkipInitiatorException:
	add     r4,#InitiatorBonus

	EndInitiatorCheck:
	mov     r0,r4
pop		{r4-r7}
pop		{r1}
bx      r1
TriangleCheck:
//we get if attacker's weapon triangle modifier is positive, if so we add TriangleBonus
push	{r4-r7,r14}
	mov     r4,r0
	mov     r5,r1
	//r4 has crit, r5 has unit struct
	//This check only makes sense if we're on an actual battle,
	//so it only executes if unit struct is a battle struct!
	mov     r0,r5
	bl      IsUnitStructBattleStruct
	cmp     r0,#0x01
	beq     TriangleCheckStart
	b       TriangleCheckEnd

	TriangleCheckStart:
	mov	    r0,#0x54
	ldrb    r0,[r5,r0]
	cmp     r0,#0x80
	bge     TriangleCheckEnd//number is negative
	cmp     r0,#0x00
	beq     TriangleCheckEnd//zero
	add     r4,#TriangleBonus
	b		TriangleCheckEnd
	
	TriangleCheckEnd:
	mov     r0,r4
pop		{r4-r7}
pop		{r1}
bx      r1

DoublingCheck:
//we get if attacker can double enemy, if so we add DoublingBonus
push	{r4-r7,r14}
	mov     r4,r0
	mov     r5,r1
	//r4 has crit, r5 has unit struct
	//This check only makes sense if we're on an actual battle,
	//so it only executes if unit struct is a battle struct!
	mov     r0,r5
	bl      IsUnitStructBattleStruct
	cmp     r0,#0x01
	beq     DoublingCheckStart
	b       DoublingCheckEnd

	DoublingCheckStart:
	//we load AS of both sides
	ldr     r0,=#AttackerBattleStruct
	blh		#0x0802AB74,r1
	ldr     r0,=#DefenderBattleStruct
	blh		#0x0802AB74,r1

	ldr     r1,=#AttackerBattleStruct
	cmp     r5,r1
	beq     DoublingCheckDefenderR0
	ldr     r0,=#AttackerBattleStruct
	ldr     r1,=#DefenderBattleStruct
	b       CheckDoubling
	DoublingCheckDefenderR0:
	ldr     r0,=#DefenderBattleStruct
	mov     r1,r5
	b       CheckDoubling
	
	CheckDoubling:
	//we compare AS of current unit vs opponent
	mov     r2,#0x5E
	ldrh 	r0,[r0,r2]//r0 is foe unit AS
	ldrh 	r1,[r1,r2]//r1 is current unit AS
	sub     r1,#0x04
	cmp     r1,r0
	blt     DoublingCheckEnd//AS is not on doubling range, can't double
	add     r4,#DoublingBonus
	b		DoublingCheckEnd
	
	DoublingCheckEnd:
	mov     r0,r4
pop		{r4-r7}
pop		{r1}
bx		r1

IsUnitStructBattleStruct:
push	{r14}
	ldr     r1,=#AttackerBattleStruct
	cmp     r0,r1
	beq     IsBattleStruct
	ldr     r1,=#DefenderBattleStruct
	cmp     r0,r1
	beq     IsBattleStruct
	b		IsNotBattleStruct
	
	IsNotBattleStruct:
	mov     r0,#0x00
	b		IsUnitStructBattleStructEnd
	
	IsBattleStruct:
	//we need to check if this is a real battle preview
	ldr     r1,=#PreBattleNumbersData
	mov     r2,#0x03//Bit 1 is "real battle", bit 2 is "batle prreview"
	ldrh    r1,[r1]
	and     r1,r2
	cmp     r1,#0x00
	beq     IsNotBattleStruct//this is not a battle!
	mov     r0,#0x01
	b       IsUnitStructBattleStructEnd
	
	IsUnitStructBattleStructEnd:
pop		{r1}
bx      r1

.align
.ltorg
.align

OutsiderValues:
