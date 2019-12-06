
a.out：     文件格式 elf64-x86-64


Disassembly of section .init:

0000000000000588 <_init>:
 588:	48 83 ec 08          	sub    $0x8,%rsp
 58c:	48 8b 05 55 0a 20 00 	mov    0x200a55(%rip),%rax        # 200fe8 <__gmon_start__>
 593:	48 85 c0             	test   %rax,%rax
 596:	74 02                	je     59a <_init+0x12>
 598:	ff d0                	callq  *%rax
 59a:	48 83 c4 08          	add    $0x8,%rsp
 59e:	c3                   	retq   

Disassembly of section .plt:

00000000000005a0 <.plt>:
 5a0:	ff 35 0a 0a 20 00    	pushq  0x200a0a(%rip)        # 200fb0 <_GLOBAL_OFFSET_TABLE_+0x8>
 5a6:	ff 25 0c 0a 20 00    	jmpq   *0x200a0c(%rip)        # 200fb8 <_GLOBAL_OFFSET_TABLE_+0x10>
 5ac:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005b0 <__stack_chk_fail@plt>:
 5b0:	ff 25 0a 0a 20 00    	jmpq   *0x200a0a(%rip)        # 200fc0 <__stack_chk_fail@GLIBC_2.4>
 5b6:	68 00 00 00 00       	pushq  $0x0
 5bb:	e9 e0 ff ff ff       	jmpq   5a0 <.plt>

00000000000005c0 <printf@plt>:
 5c0:	ff 25 02 0a 20 00    	jmpq   *0x200a02(%rip)        # 200fc8 <printf@GLIBC_2.2.5>
 5c6:	68 01 00 00 00       	pushq  $0x1
 5cb:	e9 d0 ff ff ff       	jmpq   5a0 <.plt>

00000000000005d0 <gettimeofday@plt>:
 5d0:	ff 25 fa 09 20 00    	jmpq   *0x2009fa(%rip)        # 200fd0 <gettimeofday@GLIBC_2.2.5>
 5d6:	68 02 00 00 00       	pushq  $0x2
 5db:	e9 c0 ff ff ff       	jmpq   5a0 <.plt>

Disassembly of section .plt.got:

00000000000005e0 <__cxa_finalize@plt>:
 5e0:	ff 25 12 0a 20 00    	jmpq   *0x200a12(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 5e6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

00000000000005f0 <_start>:
 5f0:	31 ed                	xor    %ebp,%ebp
 5f2:	49 89 d1             	mov    %rdx,%r9
 5f5:	5e                   	pop    %rsi
 5f6:	48 89 e2             	mov    %rsp,%rdx
 5f9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 5fd:	50                   	push   %rax
 5fe:	54                   	push   %rsp
 5ff:	4c 8d 05 0a 02 00 00 	lea    0x20a(%rip),%r8        # 810 <__libc_csu_fini>
 606:	48 8d 0d 93 01 00 00 	lea    0x193(%rip),%rcx        # 7a0 <__libc_csu_init>
 60d:	48 8d 3d e6 00 00 00 	lea    0xe6(%rip),%rdi        # 6fa <main>
 614:	ff 15 c6 09 20 00    	callq  *0x2009c6(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 61a:	f4                   	hlt    
 61b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000620 <deregister_tm_clones>:
 620:	48 8d 3d e9 09 20 00 	lea    0x2009e9(%rip),%rdi        # 201010 <__TMC_END__>
 627:	55                   	push   %rbp
 628:	48 8d 05 e1 09 20 00 	lea    0x2009e1(%rip),%rax        # 201010 <__TMC_END__>
 62f:	48 39 f8             	cmp    %rdi,%rax
 632:	48 89 e5             	mov    %rsp,%rbp
 635:	74 19                	je     650 <deregister_tm_clones+0x30>
 637:	48 8b 05 9a 09 20 00 	mov    0x20099a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 63e:	48 85 c0             	test   %rax,%rax
 641:	74 0d                	je     650 <deregister_tm_clones+0x30>
 643:	5d                   	pop    %rbp
 644:	ff e0                	jmpq   *%rax
 646:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 64d:	00 00 00 
 650:	5d                   	pop    %rbp
 651:	c3                   	retq   
 652:	0f 1f 40 00          	nopl   0x0(%rax)
 656:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 65d:	00 00 00 

0000000000000660 <register_tm_clones>:
 660:	48 8d 3d a9 09 20 00 	lea    0x2009a9(%rip),%rdi        # 201010 <__TMC_END__>
 667:	48 8d 35 a2 09 20 00 	lea    0x2009a2(%rip),%rsi        # 201010 <__TMC_END__>
 66e:	55                   	push   %rbp
 66f:	48 29 fe             	sub    %rdi,%rsi
 672:	48 89 e5             	mov    %rsp,%rbp
 675:	48 c1 fe 03          	sar    $0x3,%rsi
 679:	48 89 f0             	mov    %rsi,%rax
 67c:	48 c1 e8 3f          	shr    $0x3f,%rax
 680:	48 01 c6             	add    %rax,%rsi
 683:	48 d1 fe             	sar    %rsi
 686:	74 18                	je     6a0 <register_tm_clones+0x40>
 688:	48 8b 05 61 09 20 00 	mov    0x200961(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 68f:	48 85 c0             	test   %rax,%rax
 692:	74 0c                	je     6a0 <register_tm_clones+0x40>
 694:	5d                   	pop    %rbp
 695:	ff e0                	jmpq   *%rax
 697:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 69e:	00 00 
 6a0:	5d                   	pop    %rbp
 6a1:	c3                   	retq   
 6a2:	0f 1f 40 00          	nopl   0x0(%rax)
 6a6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6ad:	00 00 00 

00000000000006b0 <__do_global_dtors_aux>:
 6b0:	80 3d 59 09 20 00 00 	cmpb   $0x0,0x200959(%rip)        # 201010 <__TMC_END__>
 6b7:	75 2f                	jne    6e8 <__do_global_dtors_aux+0x38>
 6b9:	48 83 3d 37 09 20 00 	cmpq   $0x0,0x200937(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 6c0:	00 
 6c1:	55                   	push   %rbp
 6c2:	48 89 e5             	mov    %rsp,%rbp
 6c5:	74 0c                	je     6d3 <__do_global_dtors_aux+0x23>
 6c7:	48 8b 3d 3a 09 20 00 	mov    0x20093a(%rip),%rdi        # 201008 <__dso_handle>
 6ce:	e8 0d ff ff ff       	callq  5e0 <__cxa_finalize@plt>
 6d3:	e8 48 ff ff ff       	callq  620 <deregister_tm_clones>
 6d8:	c6 05 31 09 20 00 01 	movb   $0x1,0x200931(%rip)        # 201010 <__TMC_END__>
 6df:	5d                   	pop    %rbp
 6e0:	c3                   	retq   
 6e1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 6e8:	f3 c3                	repz retq 
 6ea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000006f0 <frame_dummy>:
 6f0:	55                   	push   %rbp
 6f1:	48 89 e5             	mov    %rsp,%rbp
 6f4:	5d                   	pop    %rbp
 6f5:	e9 66 ff ff ff       	jmpq   660 <register_tm_clones>

00000000000006fa <main>:
 6fa:	55                   	push   %rbp
 6fb:	48 89 e5             	mov    %rsp,%rbp
 6fe:	48 83 ec 40          	sub    $0x40,%rsp
 702:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 709:	00 00 
 70b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 70f:	31 c0                	xor    %eax,%eax
 711:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
 715:	be 00 00 00 00       	mov    $0x0,%esi
 71a:	48 89 c7             	mov    %rax,%rdi
 71d:	e8 ae fe ff ff       	callq  5d0 <gettimeofday@plt>
 722:	c7 45 c4 00 00 00 00 	movl   $0x0,-0x3c(%rbp)
 729:	eb 10                	jmp    73b <main+0x41>
 72b:	b8 01 00 00 00       	mov    $0x1,%eax
 730:	b9 02 00 00 00       	mov    $0x2,%ecx
 735:	01 c1                	add    %eax,%ecx
 737:	83 45 c4 01          	addl   $0x1,-0x3c(%rbp)
 73b:	81 7d c4 7f 96 98 00 	cmpl   $0x98967f,-0x3c(%rbp)
 742:	7e e7                	jle    72b <main+0x31>
 744:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
 748:	be 00 00 00 00       	mov    $0x0,%esi
 74d:	48 89 c7             	mov    %rax,%rdi
 750:	e8 7b fe ff ff       	callq  5d0 <gettimeofday@plt>
 755:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
 759:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
 75d:	48 29 c2             	sub    %rax,%rdx
 760:	48 89 d0             	mov    %rdx,%rax
 763:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
 767:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
 76b:	48 89 c6             	mov    %rax,%rsi
 76e:	48 8d 3d af 00 00 00 	lea    0xaf(%rip),%rdi        # 824 <_IO_stdin_used+0x4>
 775:	b8 00 00 00 00       	mov    $0x0,%eax
 77a:	e8 41 fe ff ff       	callq  5c0 <printf@plt>
 77f:	b8 00 00 00 00       	mov    $0x0,%eax
 784:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
 788:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 78f:	00 00 
 791:	74 05                	je     798 <main+0x9e>
 793:	e8 18 fe ff ff       	callq  5b0 <__stack_chk_fail@plt>
 798:	c9                   	leaveq 
 799:	c3                   	retq   
 79a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000007a0 <__libc_csu_init>:
 7a0:	41 57                	push   %r15
 7a2:	41 56                	push   %r14
 7a4:	49 89 d7             	mov    %rdx,%r15
 7a7:	41 55                	push   %r13
 7a9:	41 54                	push   %r12
 7ab:	4c 8d 25 f6 05 20 00 	lea    0x2005f6(%rip),%r12        # 200da8 <__frame_dummy_init_array_entry>
 7b2:	55                   	push   %rbp
 7b3:	48 8d 2d f6 05 20 00 	lea    0x2005f6(%rip),%rbp        # 200db0 <__init_array_end>
 7ba:	53                   	push   %rbx
 7bb:	41 89 fd             	mov    %edi,%r13d
 7be:	49 89 f6             	mov    %rsi,%r14
 7c1:	4c 29 e5             	sub    %r12,%rbp
 7c4:	48 83 ec 08          	sub    $0x8,%rsp
 7c8:	48 c1 fd 03          	sar    $0x3,%rbp
 7cc:	e8 b7 fd ff ff       	callq  588 <_init>
 7d1:	48 85 ed             	test   %rbp,%rbp
 7d4:	74 20                	je     7f6 <__libc_csu_init+0x56>
 7d6:	31 db                	xor    %ebx,%ebx
 7d8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 7df:	00 
 7e0:	4c 89 fa             	mov    %r15,%rdx
 7e3:	4c 89 f6             	mov    %r14,%rsi
 7e6:	44 89 ef             	mov    %r13d,%edi
 7e9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 7ed:	48 83 c3 01          	add    $0x1,%rbx
 7f1:	48 39 dd             	cmp    %rbx,%rbp
 7f4:	75 ea                	jne    7e0 <__libc_csu_init+0x40>
 7f6:	48 83 c4 08          	add    $0x8,%rsp
 7fa:	5b                   	pop    %rbx
 7fb:	5d                   	pop    %rbp
 7fc:	41 5c                	pop    %r12
 7fe:	41 5d                	pop    %r13
 800:	41 5e                	pop    %r14
 802:	41 5f                	pop    %r15
 804:	c3                   	retq   
 805:	90                   	nop
 806:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 80d:	00 00 00 

0000000000000810 <__libc_csu_fini>:
 810:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000814 <_fini>:
 814:	48 83 ec 08          	sub    $0x8,%rsp
 818:	48 83 c4 08          	add    $0x8,%rsp
 81c:	c3                   	retq   
