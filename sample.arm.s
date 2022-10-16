  .arch armv7-a
  .fpu vfpv3-d16

  .text
  .global  c
  .bss
  .type  c, %object
  .size  c, 20
a:
  .space  4
  .global  b
  .type  b, %object
  .size  b, 4
b:
  .space  4
  .global  c
  .data
  .type  c, %object
  .size  c, 20
c:
  @ 浮点数1-5在内存中存放的地址按照“IEEE 754 标准”单精度格式编码的浮点数，对应的十进制值
  .word  1065353216
  .word  1073741824
  .word  1077936128
  .word  1082130432
  .word  1084227584
  .text
  .global  f
  .type  f, %function
@ 函数部分
f:
  sub  sp, sp, #12
  str  r0, [sp, #4]
  str  r1, [sp]
  ldr  r2, [sp, #4]
  ldr  r3, [sp]
  cmp  r2, r3
  ble  .L1
  movs  r3, #1
  b  .end
.L1:
  movs  r3, #0
.end:
  mov  r0, r3
  adds  sp, sp, #12
  mov pc, lr
@ 读入字符串
.str0:
  .ascii  "%d%d\000"
  .align  2
.str1:
  .ascii  "%d\012\000"
  .text
  .global  main
  .syntax unified
  .thumb
  .type  main, %function
@ 主函数
main:
  push  {fp, lr}
  sub  sp, sp, #16
  add  fp, sp, #0
  ldr  r3, ._bridge
.LPIC0:
  add  r3, pc
  mov  r2, r3
  ldr  r3, ._bridge+4
.LPIC1:
  add  r3, pc
  mov  r1, r3
  ldr  r3, ._bridge+8
.LPIC2:
  add  r3, pc
  mov  r0, r3
  bl  scanf
  movs  r3, #0
  str  r3, [fp, #4]
  ldr  r3, ._bridge+12
.LPIC3:
  add  r3, pc
  ldr  r2, [r3]
  ldr  r3, ._bridge+16
.LPIC4:
  add  r3, pc
  ldr  r3, [r3]
  mov  r1, r3
  mov  r0, r2
  bl  f
  str  r0, [fp, #12]
  ldr  r3, [fp, #12]
  cmp  r3, #1
  bne  .L5
  movs  r3, #0
  str  r3, [fp, #8]
  b  .L6
.L7:
  ldr  r3, [fp, #12]
  vmov  s15, r3
  vcvt.f32.s32  s14, s15
  ldr  r2, ._bridge+20
.LPIC5:
  add  r2, pc
  ldr  r3, [fp, #8]
  lsls  r3, r3, #2
  add  r3, r3, r2
  vldr.32  s15, [r3]
  vmul.f32  s15, s14, s15
  vcvt.s32.f32  s15, s15
  vmov  r3, s15
  str  r3, [fp, #12]
  ldr  r3, [fp, #8]
  adds  r3, r3, #1
  str  r3, [fp, #8]
.L6:
  ldr  r3, [fp, #8]
  cmp  r3, #4
  ble  .L7
  ldr  r1, [fp, #12]
  ldr  r3, ._bridge+24
.LPIC6:
  add  r3, pc
  mov  r0, r3
  bl  printf
  b  .L8
.L5:
  movs  r3, #1
  str  r3, [fp, #12]
.L8:
  ldr  r1, [fp, #4]
  ldr  r3, ._bridge+28
.LPIC7:
  add  r3, pc
  mov  r0, r3
  bl  printf
  movs  r3, #0
  mov  r0, r3
  adds  fp, fp, #16
  mov  sp, fp
  pop  {fp, pc}
@ 桥接隐性全局变量的地址
._bridge:
  .word  b-(.LPIC0+4)
  .word  a-(.LPIC1+4)
  .word  .str0-(.LPIC2+4)
  .word  a-(.LPIC3+4)
  .word  b-(.LPIC4+4)
  .word  c-(.LPIC5+4)
  .word  .str1-(.LPIC6+4)
  .word  .str1-(.LPIC7+4)
  .size  main, .-main
