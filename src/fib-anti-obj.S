
fib.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 20          	sub    $0x20,%rsp
   8:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
   f:	00 00 
  11:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  15:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%rbp)
  1c:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
  23:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
  2a:	c7 45 e8 01 00 00 00 	movl   $0x1,-0x18(%rbp)
  31:	48 bf 00 00 00 00 00 	movabs $0x0,%rdi
  38:	00 00 00 
  3b:	48 8d 75 ec          	lea    -0x14(%rbp),%rsi
  3f:	48 8d 55 f4          	lea    -0xc(%rbp),%rdx
  43:	48 8d 4d f0          	lea    -0x10(%rbp),%rcx
  47:	b0 00                	mov    $0x0,%al
  49:	e8 00 00 00 00       	call   4e <main+0x4e>
  4e:	8b 45 e8             	mov    -0x18(%rbp),%eax
  51:	3b 45 ec             	cmp    -0x14(%rbp),%eax
  54:	7d 34                	jge    8a <main+0x8a>
  56:	8b 45 f0             	mov    -0x10(%rbp),%eax
  59:	89 45 e4             	mov    %eax,-0x1c(%rbp)
  5c:	8b 45 f4             	mov    -0xc(%rbp),%eax
  5f:	03 45 f0             	add    -0x10(%rbp),%eax
  62:	89 45 f0             	mov    %eax,-0x10(%rbp)
  65:	8b 75 f0             	mov    -0x10(%rbp),%esi
  68:	48 bf 00 00 00 00 00 	movabs $0x0,%rdi
  6f:	00 00 00 
  72:	b0 00                	mov    $0x0,%al
  74:	e8 00 00 00 00       	call   79 <main+0x79>
  79:	8b 45 e4             	mov    -0x1c(%rbp),%eax
  7c:	89 45 f4             	mov    %eax,-0xc(%rbp)
  7f:	8b 45 e8             	mov    -0x18(%rbp),%eax
  82:	83 c0 01             	add    $0x1,%eax
  85:	89 45 e8             	mov    %eax,-0x18(%rbp)
  88:	eb c4                	jmp    4e <main+0x4e>
  8a:	8b 45 e0             	mov    -0x20(%rbp),%eax
  8d:	64 48 8b 0c 25 28 00 	mov    %fs:0x28,%rcx
  94:	00 00 
  96:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  9a:	48 39 d1             	cmp    %rdx,%rcx
  9d:	75 06                	jne    a5 <main+0xa5>
  9f:	48 83 c4 20          	add    $0x20,%rsp
  a3:	5d                   	pop    %rbp
  a4:	c3                   	ret
  a5:	e8 00 00 00 00       	call   aa <main+0xaa>
