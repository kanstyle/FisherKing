.thumb

.macro blh to, reg
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.global prSupportSixGaleforceMove
.type prSupportSixGaleforceMove, %function

@inputs:
@  r0 = stat value
@  r1 = unit pointer
@outputs:
@  r0 = stat value
prSupportSixGaleforceMove:
    push {r4-r5, lr}
    mov  r4, r0     @r4 = stat value
    mov  r5, r1     @r5 = unit pointer

    @Make sure we're a player unit
    ldrb  r0, [r5, #0xB]
    mov   r1, #0xC0
    tst   r0, r1
    bne   End
	
	@check if already powerstaffed; if so can't trigger again
	ldr	r0, [r5,#0x0C]	@status bitfield
	mov	r1, #0x04
	lsl	r1, #0x08
	and	r0, r1
	cmp	r0, #0x00
	beq	End
	
        @Get support 6, put it in r0
		mov r3, #0x37
		ldrb r0, [r5, r3]
		mov r1, #0 @check if 0, if so keep base movement
		cmp r0, r1
		beq End
		
		@Get support 6, put it in r0
		ldrb r0, [r5, r3]
		mov r1, #255 @check if 255
		cmp r0, r1
		beq Finish
		
		ldrb r4, [r5, r3] @otherwise put support 6 in r4
		b End
		
		Finish:
		mov r4, #0
		b End

    End:
    mov  r0, r4
    mov  r1, r5
    pop  {r4-r5}
    pop  {r2}
    bx   r2

.ltorg
.align
