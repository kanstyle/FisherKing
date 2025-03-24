.thumb

.macro blh to, reg
  ldr \reg, =\to
  mov lr, \reg
 .short 0xf800
.endm

.set ValueToCritical,		OutsiderValues

mov		r1,r4
add     r1,#0x6A
mov     r3,#0x00
ldsh    r2,[r1,r3]
ldr     r3,#ValueToCritical
cmp     r2,r3
bge     Blue
//gray
mov     r1,#0x01
b       End
Blue:
mov     r1,#0x02
End:
ldr     r3,=#0x08036936|1
bx      r3
.align
.ltorg
.align

OutsiderValues:
