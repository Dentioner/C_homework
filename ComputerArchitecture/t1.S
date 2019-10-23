
t1:     file format elf32-tradlittlemips


Disassembly of section .init:

0040042c <_init>:
  40042c:	3c1c0002 	lui	gp,0x2
  400430:	279c8484 	addiu	gp,gp,-31612
  400434:	0399e021 	addu	gp,gp,t9
  400438:	27bdffe0 	addiu	sp,sp,-32
  40043c:	afbf001c 	sw	ra,28(sp)
  400440:	afbc0010 	sw	gp,16(sp)
  400444:	8f828044 	lw	v0,-32700(gp)
  400448:	00000000 	nop
  40044c:	10400007 	beqz	v0,40046c <_init+0x40>
  400450:	00000000 	nop
  400454:	8f998044 	lw	t9,-32700(gp)
  400458:	00000000 	nop
  40045c:	0320f809 	jalr	t9
  400460:	00000000 	nop
  400464:	8fbc0010 	lw	gp,16(sp)
  400468:	00000000 	nop
  40046c:	04110001 	bal	400474 <_init+0x48>
  400470:	00000000 	nop
  400474:	3c1c0042 	lui	gp,0x42
  400478:	279c88b0 	addiu	gp,gp,-30544
  40047c:	8f998018 	lw	t9,-32744(gp)
  400480:	00000000 	nop
  400484:	273905f0 	addiu	t9,t9,1520
  400488:	0320f809 	jalr	t9
  40048c:	00000000 	nop
  400490:	8fbc0010 	lw	gp,16(sp)
  400494:	00000000 	nop
  400498:	04110001 	bal	4004a0 <_init+0x74>
  40049c:	00000000 	nop
  4004a0:	3c1c0042 	lui	gp,0x42
  4004a4:	279c88b0 	addiu	gp,gp,-30544
  4004a8:	8f998018 	lw	t9,-32744(gp)
  4004ac:	00000000 	nop
  4004b0:	27390790 	addiu	t9,t9,1936
  4004b4:	0320f809 	jalr	t9
  4004b8:	00000000 	nop
  4004bc:	8fbc0010 	lw	gp,16(sp)
  4004c0:	00000000 	nop
  4004c4:	8fbf001c 	lw	ra,28(sp)
  4004c8:	00000000 	nop
  4004cc:	03e00008 	jr	ra
  4004d0:	27bd0020 	addiu	sp,sp,32

Disassembly of section .text:

004004e0 <__start>:
  4004e0:	03e00021 	move	zero,ra
  4004e4:	04110001 	bal	4004ec <__start+0xc>
  4004e8:	00000000 	nop
  4004ec:	3c1c0042 	lui	gp,0x42
  4004f0:	279c88b0 	addiu	gp,gp,-30544
  4004f4:	0000f821 	move	ra,zero
  4004f8:	8f848030 	lw	a0,-32720(gp)
  4004fc:	8fa50000 	lw	a1,0(sp)
  400500:	27a60004 	addiu	a2,sp,4
  400504:	2401fff8 	li	at,-8
  400508:	03a1e824 	and	sp,sp,at
  40050c:	27bdffe0 	addiu	sp,sp,-32
  400510:	8f878038 	lw	a3,-32712(gp)
  400514:	8f888048 	lw	t0,-32696(gp)
  400518:	00000000 	nop
  40051c:	afa80010 	sw	t0,16(sp)
  400520:	afa20014 	sw	v0,20(sp)
  400524:	afbd0018 	sw	sp,24(sp)
  400528:	8f99803c 	lw	t9,-32708(gp)
  40052c:	00000000 	nop
  400530:	0320f809 	jalr	t9
  400534:	00000000 	nop

00400538 <hlt>:
  400538:	1000ffff 	b	400538 <hlt>
  40053c:	00000000 	nop

00400540 <__do_global_dtors_aux>:
  400540:	27bdffd0 	addiu	sp,sp,-48
  400544:	afbf002c 	sw	ra,44(sp)
  400548:	afb30028 	sw	s3,40(sp)
  40054c:	afb20024 	sw	s2,36(sp)
  400550:	afb10020 	sw	s1,32(sp)
  400554:	afb0001c 	sw	s0,28(sp)
  400558:	afbc0010 	sw	gp,16(sp)
  40055c:	3c130041 	lui	s3,0x41
  400560:	92620900 	lbu	v0,2304(s3)
  400564:	00000000 	nop
  400568:	1440001a 	bnez	v0,4005d4 <__do_global_dtors_aux+0x94>
  40056c:	3c020041 	lui	v0,0x41
  400570:	3c030041 	lui	v1,0x41
  400574:	24520888 	addiu	s2,v0,2184
  400578:	2463088c 	addiu	v1,v1,2188
  40057c:	00721823 	subu	v1,v1,s2
  400580:	3c100041 	lui	s0,0x41
  400584:	00031883 	sra	v1,v1,0x2
  400588:	8e040904 	lw	a0,2308(s0)
  40058c:	2471ffff 	addiu	s1,v1,-1
  400590:	0091102b 	sltu	v0,a0,s1
  400594:	1040000e 	beqz	v0,4005d0 <__do_global_dtors_aux+0x90>
  400598:	24020001 	li	v0,1
  40059c:	24830001 	addiu	v1,a0,1
  4005a0:	00031080 	sll	v0,v1,0x2
  4005a4:	00521021 	addu	v0,v0,s2
  4005a8:	8c590000 	lw	t9,0(v0)
  4005ac:	00000000 	nop
  4005b0:	0320f809 	jalr	t9
  4005b4:	ae030904 	sw	v1,2308(s0)
  4005b8:	8e040904 	lw	a0,2308(s0)
  4005bc:	8fbc0010 	lw	gp,16(sp)
  4005c0:	0091102b 	sltu	v0,a0,s1
  4005c4:	1440fff6 	bnez	v0,4005a0 <__do_global_dtors_aux+0x60>
  4005c8:	24830001 	addiu	v1,a0,1
  4005cc:	24020001 	li	v0,1
  4005d0:	a2620900 	sb	v0,2304(s3)
  4005d4:	8fbf002c 	lw	ra,44(sp)
  4005d8:	8fb30028 	lw	s3,40(sp)
  4005dc:	8fb20024 	lw	s2,36(sp)
  4005e0:	8fb10020 	lw	s1,32(sp)
  4005e4:	8fb0001c 	lw	s0,28(sp)
  4005e8:	03e00008 	jr	ra
  4005ec:	27bd0030 	addiu	sp,sp,48

004005f0 <frame_dummy>:
  4005f0:	3c040041 	lui	a0,0x41
  4005f4:	8c820890 	lw	v0,2192(a0)
  4005f8:	3c1c0042 	lui	gp,0x42
  4005fc:	10400007 	beqz	v0,40061c <frame_dummy+0x2c>
  400600:	279c88b0 	addiu	gp,gp,-30544
  400604:	8f998040 	lw	t9,-32704(gp)
  400608:	00000000 	nop
  40060c:	13200003 	beqz	t9,40061c <frame_dummy+0x2c>
  400610:	00000000 	nop
  400614:	03200008 	jr	t9
  400618:	24840890 	addiu	a0,a0,2192
  40061c:	03e00008 	jr	ra
  400620:	00000000 	nop
	...

00400630 <leaf>:
lw	v1,20(sp)
addu	a0,a1,a0
addu	a0,a0,a2
addu	a0,a0,a3
lw	v0,16(sp)
blez	v1,400650 <leaf+0x20>
addu	v0,a0,v0
addu	v0,v0,v1
jr	ra
nop

00400658 <main>:
addiu	sp,sp,-40
sw	ra,36(sp)
lui	gp,0x42
addiu	gp,gp,-30544
sw	gp,24(sp)
li	v0,5
sw	v0,16(sp)
li	v0,6
sw	v0,20(sp)
li	a0,1
li	a1,2
li	a2,3
jal	400630 <leaf>
li	a3,4
lw	gp,24(sp)
lui	a0,0x40
addiu	a0,a0,2168
lw	t9,-32716(gp)
nop
jalr	t9
move	a1,v0
lw	gp,24(sp)
move	v0,zero
lw	ra,36(sp)
nop
jr	ra
addiu	sp,sp,40
	...

004006d0 <__libc_csu_fini>:
  4006d0:	03e00008 	jr	ra
  4006d4:	00000000 	nop

004006d8 <__libc_csu_init>:
  4006d8:	3c1c0002 	lui	gp,0x2
  4006dc:	279c81d8 	addiu	gp,gp,-32296
  4006e0:	0399e021 	addu	gp,gp,t9
  4006e4:	27bdffc8 	addiu	sp,sp,-56
  4006e8:	afbf0034 	sw	ra,52(sp)
  4006ec:	afb50030 	sw	s5,48(sp)
  4006f0:	afb4002c 	sw	s4,44(sp)
  4006f4:	afb30028 	sw	s3,40(sp)
  4006f8:	afb20024 	sw	s2,36(sp)
  4006fc:	afb10020 	sw	s1,32(sp)
  400700:	afb0001c 	sw	s0,28(sp)
  400704:	afbc0010 	sw	gp,16(sp)
  400708:	8f99802c 	lw	t9,-32724(gp)
  40070c:	0080a821 	move	s5,a0
  400710:	00a0a021 	move	s4,a1
  400714:	0320f809 	jalr	t9
  400718:	00c09821 	move	s3,a2
  40071c:	8fbc0010 	lw	gp,16(sp)
  400720:	00000000 	nop
  400724:	8f83801c 	lw	v1,-32740(gp)
  400728:	8f82801c 	lw	v0,-32740(gp)
  40072c:	00000000 	nop
  400730:	00431023 	subu	v0,v0,v1
  400734:	00029083 	sra	s2,v0,0x2
  400738:	1240000c 	beqz	s2,40076c <__libc_csu_init+0x94>
  40073c:	00608821 	move	s1,v1
  400740:	00008021 	move	s0,zero
  400744:	8e390000 	lw	t9,0(s1)
  400748:	26100001 	addiu	s0,s0,1
  40074c:	02a02021 	move	a0,s5
  400750:	02802821 	move	a1,s4
  400754:	0320f809 	jalr	t9
  400758:	02603021 	move	a2,s3
  40075c:	0212102b 	sltu	v0,s0,s2
  400760:	8fbc0010 	lw	gp,16(sp)
  400764:	1440fff7 	bnez	v0,400744 <__libc_csu_init+0x6c>
  400768:	26310004 	addiu	s1,s1,4
  40076c:	8fbf0034 	lw	ra,52(sp)
  400770:	8fb50030 	lw	s5,48(sp)
  400774:	8fb4002c 	lw	s4,44(sp)
  400778:	8fb30028 	lw	s3,40(sp)
  40077c:	8fb20024 	lw	s2,36(sp)
  400780:	8fb10020 	lw	s1,32(sp)
  400784:	8fb0001c 	lw	s0,28(sp)
  400788:	03e00008 	jr	ra
  40078c:	27bd0038 	addiu	sp,sp,56

00400790 <__do_global_ctors_aux>:
  400790:	27bdffd8 	addiu	sp,sp,-40
  400794:	afbf0024 	sw	ra,36(sp)
  400798:	afb10020 	sw	s1,32(sp)
  40079c:	afb0001c 	sw	s0,28(sp)
  4007a0:	afbc0010 	sw	gp,16(sp)
  4007a4:	3c020041 	lui	v0,0x41
  4007a8:	8c590880 	lw	t9,2176(v0)
  4007ac:	2403ffff 	li	v1,-1
  4007b0:	13230008 	beq	t9,v1,4007d4 <__do_global_ctors_aux+0x44>
  4007b4:	24500880 	addiu	s0,v0,2176
  4007b8:	2411ffff 	li	s1,-1
  4007bc:	0320f809 	jalr	t9
  4007c0:	2610fffc 	addiu	s0,s0,-4
  4007c4:	8e190000 	lw	t9,0(s0)
  4007c8:	8fbc0010 	lw	gp,16(sp)
  4007cc:	1731fffb 	bne	t9,s1,4007bc <__do_global_ctors_aux+0x2c>
  4007d0:	00000000 	nop
  4007d4:	8fbf0024 	lw	ra,36(sp)
  4007d8:	8fb10020 	lw	s1,32(sp)
  4007dc:	8fb0001c 	lw	s0,28(sp)
  4007e0:	03e00008 	jr	ra
  4007e4:	27bd0028 	addiu	sp,sp,40
	...

Disassembly of section .MIPS.stubs:

004007f0 <.MIPS.stubs>:
  4007f0:	8f998010 	lw	t9,-32752(gp)
  4007f4:	03e07821 	move	t7,ra
  4007f8:	0320f809 	jalr	t9
  4007fc:	24180008 	li	t8,8
  400800:	8f998010 	lw	t9,-32752(gp)
  400804:	03e07821 	move	t7,ra
  400808:	0320f809 	jalr	t9
  40080c:	24180006 	li	t8,6
	...

Disassembly of section .fini:

00400820 <_fini>:
  400820:	3c1c0002 	lui	gp,0x2
  400824:	279c8090 	addiu	gp,gp,-32624
  400828:	0399e021 	addu	gp,gp,t9
  40082c:	27bdffe0 	addiu	sp,sp,-32
  400830:	afbf001c 	sw	ra,28(sp)
  400834:	afbc0010 	sw	gp,16(sp)
  400838:	04110001 	bal	400840 <_fini+0x20>
  40083c:	00000000 	nop
  400840:	3c1c0042 	lui	gp,0x42
  400844:	279c88b0 	addiu	gp,gp,-30544
  400848:	8f998018 	lw	t9,-32744(gp)
  40084c:	00000000 	nop
  400850:	27390540 	addiu	t9,t9,1344
  400854:	0320f809 	jalr	t9
  400858:	00000000 	nop
  40085c:	8fbc0010 	lw	gp,16(sp)
  400860:	00000000 	nop
  400864:	8fbf001c 	lw	ra,28(sp)
  400868:	00000000 	nop
  40086c:	03e00008 	jr	ra
  400870:	27bd0020 	addiu	sp,sp,32
