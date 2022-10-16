.arch armv7-a
.fpu vfpv3-d16

.text
.global trace
.bss
.type trace, %object
.size trace, 72
trace:
.space 72

.global ptrace
.section .data.rel.local,"aw"
.type ptrace, %object
.size ptrace, 4
ptrace:
.word trace

.global nonsense
.type nonsense, %object
.size nonsense, 4
nonsense:
.word trace

.global ns
.type ns, %object
.size ns, 4
ns:
.word trace

.global nnss
.bss
.type nnss, %object
.size nnss, 480
nnss:
.space 480

.text
.global fib
.type fib, %function
fib:
  str fp, [sp, #-4]!
  add fp, sp, #0
  sub sp, sp, #28
  str r0, [fp, #-16]
  @ int n
  str r1, [fp, #-20]
  @ int* a
  str r2, [fp, #-24]
  @ int* b
  mov r3, #0
  str r3, [fp, #-8]
  @ int i = 0
  b .L0
  @ for (int i = 0; i < n; ++i)
.L1:
  ldr r3, [fp, #-24]
  @ b
  ldr r3, [r3]
  @ *b
  str r3, [fp, #-12]
  @ int t = *b
  ldr r2, [fp, #-20]
  @ a
  ldr r2, [r2]
  @ *a
  add r3, r2, r3
  ldr r2, [fp, #-24]
  str r3, [r2]
  @ *b += *a
  ldr r1, [r2]
  @ *b
  ldr r2, [fp, #-12]
  @ *a = t
  ldr r3, .LTR
.LPIC0:
  add r3, pc, r3
  ldr r2, [fp, #-8]
  @ i
  str r1, [r3, r2, lsl #2]
  @ trace[i] = *b
  ldr r3, [fp, #-20]
  @ a
  ldr r2, [fp, #-12]
  @ t
  str r2, [r3]
  @ *a = t
  ldr r3, [fp, #-8]
  add r3, r3, #1
  str r3, [fp, #-8]
  @ ++i
.L0:
  ldr r2, [fp, #-8]
  @ i
  ldr r3, [fp, #-16]
  @ n
  cmp r2, r3
  blt .L1
  @ for (...; i < n; ...)
  add sp, fp, #0
  ldr fp, [sp], #4
  bx lr
  @ return

.LTR:
.word trace-(.LPIC0+8)
.size fib, .-fib
.align 2

.global __aeabi_idiv
.section .rodata
.align 2

.LUNMSTR0:
.ascii "%d \000"
.align 2

.LSFMT:
.ascii "%d%d\000"
.space 14
.align 2

.LPFMT:
.ascii "%d %d %d %d %d %d\012\000"
.align 2

.LINVSTR:
.ascii "invalid input\012\000"
.text
.align 2

.global main
.type main, %function
main:
  push {r4, r5, r6, fp, lr}
  add fp, sp, #16
  sub sp, sp, #92
  mov r3, #1
  str r3, [fp, #-32]
  @ a = 1
  str r3, [fp, #-36]
  @ b = 1
  mov r3, #10
  str r3, [fp, #-28]
  @ n = 10
  ldr r2, .LPICT
.LPIC1:
  add r2, pc, r2
  @ char s_fmt[19] = "%d%d"
  sub r3, fp, #56
  ldm r2, {r0, r1}
  str r0, [r3]
  add r3, r3, #4
  strb r1, [r3]
  sub r3, fp, #51
  mov r2, #0
  str r2, [r3] 
  str r2, [r3, #4] 
  str r2, [r3, #8] 
  strh r2, [r3, #12] 
  ldr r3, .LPICT+4
  @ const char p_fmt[19] = "%d %d %d %d %d %d\n"
.LPIC2:
  add r3, pc, r3
  sub ip, fp, #76
  mov lr, r3
  ldmia lr!, {r0, r1, r2, r3}
  stmia ip!, {r0, r1, r2, r3}
  ldr r3, [lr]
  strh r3, [ip] 
  add ip, ip, #2
  lsr r3, r3, #16
  strb r3, [ip]
  sub r2, fp, #36
  sub r1, fp, #32
  sub r3, fp, #56
  mov r0, r3
  bl scanf(PLT)
  @ scanf(s_fmt, &a, &b)
  ldr r3, [fp, #-32]
  cmp r3, #0
  ble .L2
  @ a > 0
  ldr r3, [fp, #-36]
  cmp r3, #0
  bgt .L3
  @ 0 < b
.L2:
  ldr r3, [fp, #-32]
  cmp r3, #0
  bge .L4
  @ a >= 0
  ldr r3, [fp, #-36]
  cmp r3, #0
  blt .LINV
  @ 0 <= b
.L4:
  ldr r3, [fp, #-32]
  cmp r3, #-1
  ble .LINV
  @ a > -1
  ldr r3, [fp, #-36]
  cmp r3, #-1
  ble .LINV
  @ -1 < b
  ldr r3, [fp, #-32]
  cmp r3, #0
  bne .L3
  @ a != 0
  ldr r3, [fp, #-36]
  cmp r3, #1
  bne .LINV
  @ 1 == b
.L3:
  @ exec fib
  sub r2, fp, #36
  @ &b
  sub r3, fp, #32
  mov r1, r3
  @ &a
  ldr r0, [fp, #-28]
  @ n
  bl fib(PLT)
  @ fib
  ldr r2, [fp, #-32]
  @ a
  ldr r3, [fp, #-36]
  @ b
  add r4, r2, r3
  @ a + b
  sub r5, r2, r3
  @ a - b
  mul r6, r2, r3
  @ a * b
  mov r1, r3
  @ r1 = b
  mov r0, r2
  @ r0 = a
  bl __aeabi_idiv(PLT)
  @ r0 /= r1 -> a / b
  mov ip, r0
  ldr r1, [fp, #-32]
  @ a
  ldr r2, [fp, #-36]
  @ b
  and r3, r1, r2
  @ a & b
  orr r2, r1, r2
  @ a | b
  sub r0, fp, #76
  @ push p_fmt
  str r2, [sp, #8]
  @ push a | b
  str r3, [sp, #4]
  @ push a & b
  str ip, [sp]
  @ push a / b
  mov r3, r6
  @ push a * b
  mov r2, r5
  @ push a - b
  mov r1, r4
  @ push a + b
  bl printf(PLT)
  mov r3, #0
  str r3, [fp, #-24]
.L5:
  ldr r3, .LPICT+8
  @ "%d "
.LPIC3:
  add r3, pc, r3
  ldr r2, [fp, #-24]
  ldr r3, [r3, r2, lsl #2]
  mov r1, r3
  @ push "%d "
  ldr r0, .LPICT+12
.LPIC4:
  add r0, pc, r0
  @ push trace[i]
  bl printf(PLT)
  ldr r3, [fp, #-24]
  add r3, r3, #1
  str r3, [fp, #-24]
  @ ++i
  cmp r3, #10
  bne .L5
  @ i != 10
  mov r0, #10
  bl putchar(PLT)
  @ printf("\n") -> putchar(10)
  mov r0, #0
  b .L6
  @ printf("unreachable!\n") can be deleted
.LINV:
  ldr r3, .LPICT+16
  @ "invalid input\n"
.LPIC5:
  add r3, pc, r3
  sub ip, fp, #92
  ldm r3, {r0, r1, r2, r3}
  stmia ip!, {r0, r1, r2}
  strh r3, [ip] 
  add ip, ip, #2
  lsr r3, r3, #16
  strb r3, [ip]
  sub r3, fp, #92
  mov r0, r3
  bl printf(PLT)
  mov r0, #1
.L6:
  sub sp, fp, #16
  pop {r4, r5, r6, fp, pc}
.LPICT:
  .word .LSFMT-(.LPIC1+8)
  .word .LPFMT-(.LPIC2+8)
  .word trace-(.LPIC3+8)
  .word .LUNMSTR0-(.LPIC4+8)
  .word .LINVSTR-(.LPIC5+8)
  .size main, .-main
