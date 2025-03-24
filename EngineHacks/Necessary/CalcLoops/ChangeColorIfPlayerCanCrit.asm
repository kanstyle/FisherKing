.thumb

.macro blh to, reg
  ldr \reg, =\to
  mov lr, \reg
 .short 0xf800
.endm

.set ValueToCritical,		OutsiderValues

lsl		r1,r1,#0x01
add     r0,r6,r1
mov     r2,r4
ldr     r3,#ValueToCritical
cmp     r2,r3
bge     Blue
//gray
mov     r1,#0x01
b       End
Blue:
mov     r1,#0x02
End:
ldr     r3,=#0x080369E8|1
bx      r3
.align
.ltorg
.align

OutsiderValues:
