
fib：     文件格式 elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 c1 2f 00 00 	mov    0x2fc1(%rip),%rax        # 3fd0 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret

Disassembly of section .plt:

0000000000001020 <__stack_chk_fail@plt-0x10>:
    1020:	ff 35 ca 2f 00 00    	push   0x2fca(%rip)        # 3ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 cc 2f 00 00    	jmp    *0x2fcc(%rip)        # 3ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <__stack_chk_fail@plt>:
    1030:	ff 25 ca 2f 00 00    	jmp    *0x2fca(%rip)        # 4000 <__stack_chk_fail@GLIBC_2.4>
    1036:	68 00 00 00 00       	push   $0x0
    103b:	e9 e0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001040 <printf@plt>:
    1040:	ff 25 c2 2f 00 00    	jmp    *0x2fc2(%rip)        # 4008 <printf@GLIBC_2.2.5>
    1046:	68 01 00 00 00       	push   $0x1
    104b:	e9 d0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001050 <__isoc99_scanf@plt>:
    1050:	ff 25 ba 2f 00 00    	jmp    *0x2fba(%rip)        # 4010 <__isoc99_scanf@GLIBC_2.7>
    1056:	68 02 00 00 00       	push   $0x2
    105b:	e9 c0 ff ff ff       	jmp    1020 <_init+0x20>

Disassembly of section .text:

0000000000001060 <_start>:
    1060:	f3 0f 1e fa          	endbr64
    1064:	31 ed                	xor    %ebp,%ebp
    1066:	49 89 d1             	mov    %rdx,%r9
    1069:	5e                   	pop    %rsi
    106a:	48 89 e2             	mov    %rsp,%rdx
    106d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1071:	50                   	push   %rax
    1072:	54                   	push   %rsp
    1073:	45 31 c0             	xor    %r8d,%r8d
    1076:	31 c9                	xor    %ecx,%ecx
    1078:	48 8d 3d e1 00 00 00 	lea    0xe1(%rip),%rdi        # 1160 <main>
    107f:	ff 15 3b 2f 00 00    	call   *0x2f3b(%rip)        # 3fc0 <__libc_start_main@GLIBC_2.34>
    1085:	f4                   	hlt
    1086:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    108d:	00 00 00 

0000000000001090 <deregister_tm_clones>:
    1090:	48 8d 3d 91 2f 00 00 	lea    0x2f91(%rip),%rdi        # 4028 <__TMC_END__>
    1097:	48 8d 05 8a 2f 00 00 	lea    0x2f8a(%rip),%rax        # 4028 <__TMC_END__>
    109e:	48 39 f8             	cmp    %rdi,%rax
    10a1:	74 15                	je     10b8 <deregister_tm_clones+0x28>
    10a3:	48 8b 05 1e 2f 00 00 	mov    0x2f1e(%rip),%rax        # 3fc8 <_ITM_deregisterTMCloneTable@Base>
    10aa:	48 85 c0             	test   %rax,%rax
    10ad:	74 09                	je     10b8 <deregister_tm_clones+0x28>
    10af:	ff e0                	jmp    *%rax
    10b1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10b8:	c3                   	ret
    10b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010c0 <register_tm_clones>:
    10c0:	48 8d 3d 61 2f 00 00 	lea    0x2f61(%rip),%rdi        # 4028 <__TMC_END__>
    10c7:	48 8d 35 5a 2f 00 00 	lea    0x2f5a(%rip),%rsi        # 4028 <__TMC_END__>
    10ce:	48 29 fe             	sub    %rdi,%rsi
    10d1:	48 89 f0             	mov    %rsi,%rax
    10d4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10d8:	48 c1 f8 03          	sar    $0x3,%rax
    10dc:	48 01 c6             	add    %rax,%rsi
    10df:	48 d1 fe             	sar    %rsi
    10e2:	74 14                	je     10f8 <register_tm_clones+0x38>
    10e4:	48 8b 05 ed 2e 00 00 	mov    0x2eed(%rip),%rax        # 3fd8 <_ITM_registerTMCloneTable@Base>
    10eb:	48 85 c0             	test   %rax,%rax
    10ee:	74 08                	je     10f8 <register_tm_clones+0x38>
    10f0:	ff e0                	jmp    *%rax
    10f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10f8:	c3                   	ret
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <__do_global_dtors_aux>:
    1100:	f3 0f 1e fa          	endbr64
    1104:	80 3d 1d 2f 00 00 00 	cmpb   $0x0,0x2f1d(%rip)        # 4028 <__TMC_END__>
    110b:	75 33                	jne    1140 <__do_global_dtors_aux+0x40>
    110d:	55                   	push   %rbp
    110e:	48 83 3d ca 2e 00 00 	cmpq   $0x0,0x2eca(%rip)        # 3fe0 <__cxa_finalize@GLIBC_2.2.5>
    1115:	00 
    1116:	48 89 e5             	mov    %rsp,%rbp
    1119:	74 0d                	je     1128 <__do_global_dtors_aux+0x28>
    111b:	48 8b 3d fe 2e 00 00 	mov    0x2efe(%rip),%rdi        # 4020 <__dso_handle>
    1122:	ff 15 b8 2e 00 00    	call   *0x2eb8(%rip)        # 3fe0 <__cxa_finalize@GLIBC_2.2.5>
    1128:	e8 63 ff ff ff       	call   1090 <deregister_tm_clones>
    112d:	c6 05 f4 2e 00 00 01 	movb   $0x1,0x2ef4(%rip)        # 4028 <__TMC_END__>
    1134:	5d                   	pop    %rbp
    1135:	c3                   	ret
    1136:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    113d:	00 00 00 
    1140:	c3                   	ret
    1141:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
    1148:	00 00 00 00 
    114c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001150 <frame_dummy>:
    1150:	f3 0f 1e fa          	endbr64
    1154:	e9 67 ff ff ff       	jmp    10c0 <register_tm_clones>
    1159:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001160 <main>:
    1160:	55                   	push   %rbp
    1161:	48 89 e5             	mov    %rsp,%rbp
    1164:	48 83 ec 20          	sub    $0x20,%rsp
    1168:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    116f:	00 00 
    1171:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1175:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%rbp)
    117c:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
    1183:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
    118a:	c7 45 e8 01 00 00 00 	movl   $0x1,-0x18(%rbp)
    1191:	48 bf 04 20 00 00 00 	movabs $0x2004,%rdi
    1198:	00 00 00 
    119b:	48 8d 75 ec          	lea    -0x14(%rbp),%rsi
    119f:	48 8d 55 f4          	lea    -0xc(%rbp),%rdx
    11a3:	48 8d 4d f0          	lea    -0x10(%rbp),%rcx
    11a7:	b0 00                	mov    $0x0,%al
    11a9:	e8 a2 fe ff ff       	call   1050 <__isoc99_scanf@plt>
    11ae:	8b 45 e8             	mov    -0x18(%rbp),%eax
    11b1:	3b 45 ec             	cmp    -0x14(%rbp),%eax
    11b4:	7d 34                	jge    11ea <main+0x8a>
    11b6:	8b 45 f0             	mov    -0x10(%rbp),%eax
    11b9:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    11bc:	8b 45 f4             	mov    -0xc(%rbp),%eax
    11bf:	03 45 f0             	add    -0x10(%rbp),%eax
    11c2:	89 45 f0             	mov    %eax,-0x10(%rbp)
    11c5:	8b 75 f0             	mov    -0x10(%rbp),%esi
    11c8:	48 bf 0b 20 00 00 00 	movabs $0x200b,%rdi
    11cf:	00 00 00 
    11d2:	b0 00                	mov    $0x0,%al
    11d4:	e8 67 fe ff ff       	call   1040 <printf@plt>
    11d9:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    11dc:	89 45 f4             	mov    %eax,-0xc(%rbp)
    11df:	8b 45 e8             	mov    -0x18(%rbp),%eax
    11e2:	83 c0 01             	add    $0x1,%eax
    11e5:	89 45 e8             	mov    %eax,-0x18(%rbp)
    11e8:	eb c4                	jmp    11ae <main+0x4e>
    11ea:	8b 45 e0             	mov    -0x20(%rbp),%eax
    11ed:	64 48 8b 0c 25 28 00 	mov    %fs:0x28,%rcx
    11f4:	00 00 
    11f6:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    11fa:	48 39 d1             	cmp    %rdx,%rcx
    11fd:	75 06                	jne    1205 <main+0xa5>
    11ff:	48 83 c4 20          	add    $0x20,%rsp
    1203:	5d                   	pop    %rbp
    1204:	c3                   	ret
    1205:	e8 26 fe ff ff       	call   1030 <__stack_chk_fail@plt>

Disassembly of section .fini:

000000000000120c <_fini>:
    120c:	f3 0f 1e fa          	endbr64
    1210:	48 83 ec 08          	sub    $0x8,%rsp
    1214:	48 83 c4 08          	add    $0x8,%rsp
    1218:	c3                   	ret
