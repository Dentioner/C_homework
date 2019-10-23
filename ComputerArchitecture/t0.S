
t0:     file format elf32-tradlittlemips


Disassembly of section .init:

0040042c <_init>:
  40042c:	3c1c0002 	lui	gp,0x2
  400430:	279c8544 	addiu	gp,gp,-31420
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
  400478:	279c8970 	addiu	gp,gp,-30352
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
  4004a4:	279c8970 	addiu	gp,gp,-30352
  4004a8:	8f998018 	lw	t9,-32744(gp)
  4004ac:	00000000 	nop
  4004b0:	27390830 	addiu	t9,t9,2096
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
  4004f0:	279c8970 	addiu	gp,gp,-30352
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
  400560:	926209c0 	lbu	v0,2496(s3)
  400564:	00000000 	nop
  400568:	1440001a 	bnez	v0,4005d4 <__do_global_dtors_aux+0x94>
  40056c:	3c020041 	lui	v0,0x41
  400570:	3c030041 	lui	v1,0x41
  400574:	2452094c 	addiu	s2,v0,2380
  400578:	24630950 	addiu	v1,v1,2384
  40057c:	00721823 	subu	v1,v1,s2
  400580:	3c100041 	lui	s0,0x41
  400584:	00031883 	sra	v1,v1,0x2
  400588:	8e0409c4 	lw	a0,2500(s0)
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
  4005b4:	ae0309c4 	sw	v1,2500(s0)
  4005b8:	8e0409c4 	lw	a0,2500(s0)
  4005bc:	8fbc0010 	lw	gp,16(sp)
  4005c0:	0091102b 	sltu	v0,a0,s1
  4005c4:	1440fff6 	bnez	v0,4005a0 <__do_global_dtors_aux+0x60>
  4005c8:	24830001 	addiu	v1,a0,1
  4005cc:	24020001 	li	v0,1
  4005d0:	a26209c0 	sb	v0,2496(s3)
  4005d4:	8fbf002c 	lw	ra,44(sp)
  4005d8:	8fb30028 	lw	s3,40(sp)
  4005dc:	8fb20024 	lw	s2,36(sp)
  4005e0:	8fb10020 	lw	s1,32(sp)
  4005e4:	8fb0001c 	lw	s0,28(sp)
  4005e8:	03e00008 	jr	ra
  4005ec:	27bd0030 	addiu	sp,sp,48

004005f0 <frame_dummy>:
  4005f0:	3c040041 	lui	a0,0x41
  4005f4:	8c820954 	lw	v0,2388(a0)
  4005f8:	3c1c0042 	lui	gp,0x42
  4005fc:	10400007 	beqz	v0,40061c <frame_dummy+0x2c>
  400600:	279c8970 	addiu	gp,gp,-30352
  400604:	8f998040 	lw	t9,-32704(gp)
  400608:	00000000 	nop
  40060c:	13200003 	beqz	t9,40061c <frame_dummy+0x2c>
  400610:	00000000 	nop
  400614:	03200008 	jr	t9
  400618:	24840954 	addiu	a0,a0,2388
  40061c:	03e00008 	jr	ra
  400620:	00000000 	nop
	...

00400630 <leaf>:
  400630:	27bdffe8 	addiu	sp,sp,-24
  400634:	afbe0014 	sw	s8,20(sp)
  400638:	03a0f021 	move	s8,sp
  40063c:	afbc0000 	sw	gp,0(sp)
  400640:	afc40018 	sw	a0,24(s8)
  400644:	afc5001c 	sw	a1,28(s8)
  400648:	afc60020 	sw	a2,32(s8)
  40064c:	afc70024 	sw	a3,36(s8)
  400650:	8fc30018 	lw	v1,24(s8)
  400654:	8fc2001c 	lw	v0,28(s8)
  400658:	00000000 	nop
  40065c:	00621821 	addu	v1,v1,v0
  400660:	8fc20020 	lw	v0,32(s8)
  400664:	00000000 	nop
  400668:	00621821 	addu	v1,v1,v0
  40066c:	8fc20024 	lw	v0,36(s8)
  400670:	00000000 	nop
  400674:	00621821 	addu	v1,v1,v0
  400678:	8fc20028 	lw	v0,40(s8)
  40067c:	00000000 	nop
  400680:	00621021 	addu	v0,v1,v0
  400684:	afc20008 	sw	v0,8(s8)
  400688:	afc0000c 	sw	zero,12(s8)
  40068c:	10000009 	b	4006b4 <leaf+0x84>
  400690:	00000000 	nop
  400694:	8fc20008 	lw	v0,8(s8)
  400698:	00000000 	nop
  40069c:	24420001 	addiu	v0,v0,1
  4006a0:	afc20008 	sw	v0,8(s8)
  4006a4:	8fc2000c 	lw	v0,12(s8)
  4006a8:	00000000 	nop
  4006ac:	24420001 	addiu	v0,v0,1
  4006b0:	afc2000c 	sw	v0,12(s8)
  4006b4:	8fc2000c 	lw	v0,12(s8)
  4006b8:	8fc3002c 	lw	v1,44(s8)
  4006bc:	00000000 	nop
  4006c0:	0043102a 	slt	v0,v0,v1
  4006c4:	1440fff3 	bnez	v0,400694 <leaf+0x64>
  4006c8:	00000000 	nop
  4006cc:	8fc20008 	lw	v0,8(s8)
  4006d0:	03c0e821 	move	sp,s8
  4006d4:	8fbe0014 	lw	s8,20(sp)
  4006d8:	27bd0018 	addiu	sp,sp,24
  4006dc:	03e00008 	jr	ra
  4006e0:	00000000 	nop

004006e4 <main>:
  4006e4:	27bdffd0 	addiu	sp,sp,-48
  4006e8:	afbf002c 	sw	ra,44(sp)
  4006ec:	afbe0028 	sw	s8,40(sp)
  4006f0:	03a0f021 	move	s8,sp
  4006f4:	3c1c0042 	lui	gp,0x42
  4006f8:	279c8970 	addiu	gp,gp,-30352
  4006fc:	afbc0018 	sw	gp,24(sp)
  400700:	24020005 	li	v0,5
  400704:	afa20010 	sw	v0,16(sp)
  400708:	24020006 	li	v0,6
  40070c:	afa20014 	sw	v0,20(sp)
  400710:	24040001 	li	a0,1
  400714:	24050002 	li	a1,2
  400718:	24060003 	li	a2,3
  40071c:	24070004 	li	a3,4
  400720:	0c10018c 	jal	400630 <leaf>
  400724:	00000000 	nop
  400728:	8fdc0018 	lw	gp,24(s8)
  40072c:	afc20020 	sw	v0,32(s8)
  400730:	3c020040 	lui	v0,0x40
  400734:	24440930 	addiu	a0,v0,2352
  400738:	8fc50020 	lw	a1,32(s8)
  40073c:	8f998034 	lw	t9,-32716(gp)
  400740:	00000000 	nop
  400744:	0320f809 	jalr	t9
  400748:	00000000 	nop
  40074c:	8fdc0018 	lw	gp,24(s8)
  400750:	00001021 	move	v0,zero
  400754:	03c0e821 	move	sp,s8
  400758:	8fbf002c 	lw	ra,44(sp)
  40075c:	8fbe0028 	lw	s8,40(sp)
  400760:	27bd0030 	addiu	sp,sp,48
  400764:	03e00008 	jr	ra
  400768:	00000000 	nop
  40076c:	00000000 	nop

00400770 <__libc_csu_fini>:
  400770:	03e00008 	jr	ra
  400774:	00000000 	nop

00400778 <__libc_csu_init>:
  400778:	3c1c0002 	lui	gp,0x2
  40077c:	279c81f8 	addiu	gp,gp,-32264
  400780:	0399e021 	addu	gp,gp,t9
  400784:	27bdffc8 	addiu	sp,sp,-56
  400788:	afbf0034 	sw	ra,52(sp)
  40078c:	afb50030 	sw	s5,48(sp)
  400790:	afb4002c 	sw	s4,44(sp)
  400794:	afb30028 	sw	s3,40(sp)
  400798:	afb20024 	sw	s2,36(sp)
  40079c:	afb10020 	sw	s1,32(sp)
  4007a0:	afb0001c 	sw	s0,28(sp)
  4007a4:	afbc0010 	sw	gp,16(sp)
  4007a8:	8f99802c 	lw	t9,-32724(gp)
  4007ac:	0080a821 	move	s5,a0
  4007b0:	00a0a021 	move	s4,a1
  4007b4:	0320f809 	jalr	t9
  4007b8:	00c09821 	move	s3,a2
  4007bc:	8fbc0010 	lw	gp,16(sp)
  4007c0:	00000000 	nop
  4007c4:	8f83801c 	lw	v1,-32740(gp)
  4007c8:	8f82801c 	lw	v0,-32740(gp)
  4007cc:	00000000 	nop
  4007d0:	00431023 	subu	v0,v0,v1
  4007d4:	00029083 	sra	s2,v0,0x2
  4007d8:	1240000c 	beqz	s2,40080c <__libc_csu_init+0x94>
  4007dc:	00608821 	move	s1,v1
  4007e0:	00008021 	move	s0,zero
  4007e4:	8e390000 	lw	t9,0(s1)
  4007e8:	26100001 	addiu	s0,s0,1
  4007ec:	02a02021 	move	a0,s5
  4007f0:	02802821 	move	a1,s4
  4007f4:	0320f809 	jalr	t9
  4007f8:	02603021 	move	a2,s3
  4007fc:	0212102b 	sltu	v0,s0,s2
  400800:	8fbc0010 	lw	gp,16(sp)
  400804:	1440fff7 	bnez	v0,4007e4 <__libc_csu_init+0x6c>
  400808:	26310004 	addiu	s1,s1,4
  40080c:	8fbf0034 	lw	ra,52(sp)
  400810:	8fb50030 	lw	s5,48(sp)
  400814:	8fb4002c 	lw	s4,44(sp)
  400818:	8fb30028 	lw	s3,40(sp)
  40081c:	8fb20024 	lw	s2,36(sp)
  400820:	8fb10020 	lw	s1,32(sp)
  400824:	8fb0001c 	lw	s0,28(sp)
  400828:	03e00008 	jr	ra
  40082c:	27bd0038 	addiu	sp,sp,56

00400830 <__do_global_ctors_aux>:
  400830:	27bdffd8 	addiu	sp,sp,-40
  400834:	afbf0024 	sw	ra,36(sp)
  400838:	afb10020 	sw	s1,32(sp)
  40083c:	afb0001c 	sw	s0,28(sp)
  400840:	afbc0010 	sw	gp,16(sp)
  400844:	3c020041 	lui	v0,0x41
  400848:	8c590944 	lw	t9,2372(v0)
  40084c:	2403ffff 	li	v1,-1
  400850:	13230008 	beq	t9,v1,400874 <__do_global_ctors_aux+0x44>
  400854:	24500944 	addiu	s0,v0,2372
  400858:	2411ffff 	li	s1,-1
  40085c:	0320f809 	jalr	t9
  400860:	2610fffc 	addiu	s0,s0,-4
  400864:	8e190000 	lw	t9,0(s0)
  400868:	8fbc0010 	lw	gp,16(sp)
  40086c:	1731fffb 	bne	t9,s1,40085c <__do_global_ctors_aux+0x2c>
  400870:	00000000 	nop
  400874:	8fbf0024 	lw	ra,36(sp)
  400878:	8fb10020 	lw	s1,32(sp)
  40087c:	8fb0001c 	lw	s0,28(sp)
  400880:	03e00008 	jr	ra
  400884:	27bd0028 	addiu	sp,sp,40
	...

Disassembly of section .MIPS.stubs:

00400890 <.MIPS.stubs>:
  400890:	8f998010 	lw	t9,-32752(gp)
  400894:	03e07821 	move	t7,ra
  400898:	0320f809 	jalr	t9
  40089c:	24180008 	li	t8,8
  4008a0:	8f998010 	lw	t9,-32752(gp)
  4008a4:	03e07821 	move	t7,ra
  4008a8:	0320f809 	jalr	t9
  4008ac:	24180006 	li	t8,6
	...

Disassembly of section .fini:

004008c0 <_fini>:
  4008c0:	3c1c0002 	lui	gp,0x2
  4008c4:	279c80b0 	addiu	gp,gp,-32592
  4008c8:	0399e021 	addu	gp,gp,t9
  4008cc:	27bdffe0 	addiu	sp,sp,-32
  4008d0:	afbf001c 	sw	ra,28(sp)
  4008d4:	afbc0010 	sw	gp,16(sp)
  4008d8:	04110001 	bal	4008e0 <_fini+0x20>
  4008dc:	00000000 	nop
  4008e0:	3c1c0042 	lui	gp,0x42
  4008e4:	279c8970 	addiu	gp,gp,-30352
  4008e8:	8f998018 	lw	t9,-32744(gp)
  4008ec:	00000000 	nop
  4008f0:	27390540 	addiu	t9,t9,1344
  4008f4:	0320f809 	jalr	t9
  4008f8:	00000000 	nop
  4008fc:	8fbc0010 	lw	gp,16(sp)
  400900:	00000000 	nop
  400904:	8fbf001c 	lw	ra,28(sp)
  400908:	00000000 	nop
  40090c:	03e00008 	jr	ra
  400910:	27bd0020 	addiu	sp,sp,32
