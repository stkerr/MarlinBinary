
MarlinBinaryTestApp:     file format elf32-i386


Disassembly of section .init:

08048328 <_init>:
 8048328:	55                   	push   %ebp
 8048329:	89 e5                	mov    %esp,%ebp
 804832b:	83 ec 08             	sub    $0x8,%esp
 804832e:	e8 71 00 00 00       	call   80483a4 <call_gmon_start>
 8048333:	e8 f8 00 00 00       	call   8048430 <call_gmon_start+0x8c>
 8048338:	e8 23 04 00 00       	call   8048760 <__libc_csu_fini+0x10>
 804833d:	c9                   	leave  
 804833e:	c3                   	ret    

Disassembly of section .plt:

08048340 <__gmon_start__@plt-0x10>:
 8048340:	ff 35 f8 9f 04 08    	pushl  0x8049ff8
 8048346:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 804834c:	00 00                	add    %al,(%eax)
	...

08048350 <__gmon_start__@plt>:
 8048350:	ff 25 00 a0 04 08    	jmp    *0x804a000
 8048356:	68 00 00 00 00       	push   $0x0
 804835b:	e9 e0 ff ff ff       	jmp    8048340 <_init+0x18>

08048360 <__libc_start_main@plt>:
 8048360:	ff 25 04 a0 04 08    	jmp    *0x804a004
 8048366:	68 08 00 00 00       	push   $0x8
 804836b:	e9 d0 ff ff ff       	jmp    8048340 <_init+0x18>

08048370 <printf@plt>:
 8048370:	ff 25 08 a0 04 08    	jmp    *0x804a008
 8048376:	68 10 00 00 00       	push   $0x10
 804837b:	e9 c0 ff ff ff       	jmp    8048340 <_init+0x18>

Disassembly of section .text:

08048380 <_start>:
 8048380:	31 ed                	xor    %ebp,%ebp
 8048382:	5e                   	pop    %esi
 8048383:	89 e1                	mov    %esp,%ecx
 8048385:	83 e4 f0             	and    $0xfffffff0,%esp
 8048388:	50                   	push   %eax
 8048389:	54                   	push   %esp
 804838a:	52                   	push   %edx
 804838b:	68 50 87 04 08       	push   $0x8048750
 8048390:	68 f0 86 04 08       	push   $0x80486f0
 8048395:	51                   	push   %ecx
 8048396:	56                   	push   %esi
 8048397:	68 da 85 04 08       	push   $0x80485da
 804839c:	e8 bf ff ff ff       	call   8048360 <__libc_start_main@plt>
 80483a1:	f4                   	hlt    
 80483a2:	90                   	nop    
 80483a3:	90                   	nop    

080483a4 <call_gmon_start>:
 80483a4:	55                   	push   %ebp
 80483a5:	89 e5                	mov    %esp,%ebp
 80483a7:	53                   	push   %ebx
 80483a8:	83 ec 04             	sub    $0x4,%esp
 80483ab:	e8 00 00 00 00       	call   80483b0 <call_gmon_start+0xc>
 80483b0:	5b                   	pop    %ebx
 80483b1:	81 c3 44 1c 00 00    	add    $0x1c44,%ebx
 80483b7:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 80483bd:	85 d2                	test   %edx,%edx
 80483bf:	74 05                	je     80483c6 <call_gmon_start+0x22>
 80483c1:	e8 8a ff ff ff       	call   8048350 <__gmon_start__@plt>
 80483c6:	58                   	pop    %eax
 80483c7:	5b                   	pop    %ebx
 80483c8:	c9                   	leave  
 80483c9:	c3                   	ret    
 80483ca:	90                   	nop    
 80483cb:	90                   	nop    
 80483cc:	90                   	nop    
 80483cd:	90                   	nop    
 80483ce:	90                   	nop    
 80483cf:	90                   	nop    
 80483d0:	55                   	push   %ebp
 80483d1:	89 e5                	mov    %esp,%ebp
 80483d3:	53                   	push   %ebx
 80483d4:	83 ec 04             	sub    $0x4,%esp
 80483d7:	80 3d 14 a0 04 08 00 	cmpb   $0x0,0x804a014
 80483de:	75 3d                	jne    804841d <call_gmon_start+0x79>
 80483e0:	a1 18 a0 04 08       	mov    0x804a018,%eax
 80483e5:	bb 00 9f 04 08       	mov    $0x8049f00,%ebx
 80483ea:	81 eb fc 9e 04 08    	sub    $0x8049efc,%ebx
 80483f0:	c1 fb 02             	sar    $0x2,%ebx
 80483f3:	4b                   	dec    %ebx
 80483f4:	39 d8                	cmp    %ebx,%eax
 80483f6:	73 1e                	jae    8048416 <call_gmon_start+0x72>
 80483f8:	90                   	nop    
 80483f9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8048400:	40                   	inc    %eax
 8048401:	a3 18 a0 04 08       	mov    %eax,0x804a018
 8048406:	ff 14 85 fc 9e 04 08 	call   *0x8049efc(,%eax,4)
 804840d:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048412:	39 d8                	cmp    %ebx,%eax
 8048414:	72 ea                	jb     8048400 <call_gmon_start+0x5c>
 8048416:	c6 05 14 a0 04 08 01 	movb   $0x1,0x804a014
 804841d:	83 c4 04             	add    $0x4,%esp
 8048420:	5b                   	pop    %ebx
 8048421:	c9                   	leave  
 8048422:	c3                   	ret    
 8048423:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048429:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
 8048430:	55                   	push   %ebp
 8048431:	89 e5                	mov    %esp,%ebp
 8048433:	83 ec 18             	sub    $0x18,%esp
 8048436:	a1 04 9f 04 08       	mov    0x8049f04,%eax
 804843b:	85 c0                	test   %eax,%eax
 804843d:	74 12                	je     8048451 <call_gmon_start+0xad>
 804843f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048444:	85 c0                	test   %eax,%eax
 8048446:	74 09                	je     8048451 <call_gmon_start+0xad>
 8048448:	c7 04 24 04 9f 04 08 	movl   $0x8049f04,(%esp)
 804844f:	ff d0                	call   *%eax
 8048451:	c9                   	leave  
 8048452:	c3                   	ret    
 8048453:	90                   	nop    

08048454 <_Z9function1v>:
 8048454:	55                   	push   %ebp
 8048455:	89 e5                	mov    %esp,%ebp
 8048457:	83 ec 10             	sub    $0x10,%esp
 804845a:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%ebp)
 8048461:	c7 45 f8 01 00 00 00 	movl   $0x1,-0x8(%ebp)
 8048468:	8b 45 fc             	mov    -0x4(%ebp),%eax
 804846b:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 804846f:	c9                   	leave  
 8048470:	c3                   	ret    

08048471 <_Z9function2v>:
 8048471:	55                   	push   %ebp
 8048472:	89 e5                	mov    %esp,%ebp
 8048474:	83 ec 10             	sub    $0x10,%esp
 8048477:	c7 45 fc 02 00 00 00 	movl   $0x2,-0x4(%ebp)
 804847e:	c7 45 f8 02 00 00 00 	movl   $0x2,-0x8(%ebp)
 8048485:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048488:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 804848c:	c9                   	leave  
 804848d:	c3                   	ret    

0804848e <_Z9function3v>:
 804848e:	55                   	push   %ebp
 804848f:	89 e5                	mov    %esp,%ebp
 8048491:	83 ec 10             	sub    $0x10,%esp
 8048494:	c7 45 fc 03 00 00 00 	movl   $0x3,-0x4(%ebp)
 804849b:	c7 45 f8 03 00 00 00 	movl   $0x3,-0x8(%ebp)
 80484a2:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484a5:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 80484a9:	c9                   	leave  
 80484aa:	c3                   	ret    

080484ab <_Z9function4v>:
 80484ab:	55                   	push   %ebp
 80484ac:	89 e5                	mov    %esp,%ebp
 80484ae:	83 ec 10             	sub    $0x10,%esp
 80484b1:	c7 45 fc 04 00 00 00 	movl   $0x4,-0x4(%ebp)
 80484b8:	c7 45 f8 04 00 00 00 	movl   $0x4,-0x8(%ebp)
 80484bf:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484c2:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 80484c6:	c9                   	leave  
 80484c7:	c3                   	ret    

080484c8 <_Z9function5v>:
 80484c8:	55                   	push   %ebp
 80484c9:	89 e5                	mov    %esp,%ebp
 80484cb:	83 ec 10             	sub    $0x10,%esp
 80484ce:	c7 45 fc 05 00 00 00 	movl   $0x5,-0x4(%ebp)
 80484d5:	c7 45 f8 05 00 00 00 	movl   $0x5,-0x8(%ebp)
 80484dc:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484df:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 80484e3:	c9                   	leave  
 80484e4:	c3                   	ret    

080484e5 <_Z9function6v>:
 80484e5:	55                   	push   %ebp
 80484e6:	89 e5                	mov    %esp,%ebp
 80484e8:	83 ec 10             	sub    $0x10,%esp
 80484eb:	c7 45 fc 06 00 00 00 	movl   $0x6,-0x4(%ebp)
 80484f2:	c7 45 f8 06 00 00 00 	movl   $0x6,-0x8(%ebp)
 80484f9:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484fc:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 8048500:	c9                   	leave  
 8048501:	c3                   	ret    

08048502 <_Z9function7v>:
 8048502:	55                   	push   %ebp
 8048503:	89 e5                	mov    %esp,%ebp
 8048505:	83 ec 10             	sub    $0x10,%esp
 8048508:	c7 45 fc 07 00 00 00 	movl   $0x7,-0x4(%ebp)
 804850f:	c7 45 f8 07 00 00 00 	movl   $0x7,-0x8(%ebp)
 8048516:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048519:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 804851d:	c9                   	leave  
 804851e:	c3                   	ret    

0804851f <_Z9function8v>:
 804851f:	55                   	push   %ebp
 8048520:	89 e5                	mov    %esp,%ebp
 8048522:	83 ec 10             	sub    $0x10,%esp
 8048525:	c7 45 fc 08 00 00 00 	movl   $0x8,-0x4(%ebp)
 804852c:	c7 45 f8 08 00 00 00 	movl   $0x8,-0x8(%ebp)
 8048533:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048536:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 804853a:	c9                   	leave  
 804853b:	c3                   	ret    

0804853c <_Z9function9v>:
 804853c:	55                   	push   %ebp
 804853d:	89 e5                	mov    %esp,%ebp
 804853f:	83 ec 10             	sub    $0x10,%esp
 8048542:	c7 45 fc 09 00 00 00 	movl   $0x9,-0x4(%ebp)
 8048549:	c7 45 f8 09 00 00 00 	movl   $0x9,-0x8(%ebp)
 8048550:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048553:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 8048557:	c9                   	leave  
 8048558:	c3                   	ret    

08048559 <_Z10function10v>:
 8048559:	55                   	push   %ebp
 804855a:	89 e5                	mov    %esp,%ebp
 804855c:	83 ec 10             	sub    $0x10,%esp
 804855f:	c7 45 fc 0a 00 00 00 	movl   $0xa,-0x4(%ebp)
 8048566:	c7 45 f8 0a 00 00 00 	movl   $0xa,-0x8(%ebp)
 804856d:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048570:	0f af 45 f8          	imul   -0x8(%ebp),%eax
 8048574:	c9                   	leave  
 8048575:	c3                   	ret    

08048576 <_Z14jump_function1v>:
 8048576:	55                   	push   %ebp
 8048577:	89 e5                	mov    %esp,%ebp
 8048579:	e8 d6 fe ff ff       	call   8048454 <_Z9function1v>
 804857e:	5d                   	pop    %ebp
 804857f:	c3                   	ret    

08048580 <_Z14jump_function2v>:
 8048580:	55                   	push   %ebp
 8048581:	89 e5                	mov    %esp,%ebp
 8048583:	e8 e9 fe ff ff       	call   8048471 <_Z9function2v>
 8048588:	5d                   	pop    %ebp
 8048589:	c3                   	ret    

0804858a <_Z14jump_function3v>:
 804858a:	55                   	push   %ebp
 804858b:	89 e5                	mov    %esp,%ebp
 804858d:	e8 fc fe ff ff       	call   804848e <_Z9function3v>
 8048592:	5d                   	pop    %ebp
 8048593:	c3                   	ret    

08048594 <_Z14jump_function4v>:
 8048594:	55                   	push   %ebp
 8048595:	89 e5                	mov    %esp,%ebp
 8048597:	e8 0f ff ff ff       	call   80484ab <_Z9function4v>
 804859c:	5d                   	pop    %ebp
 804859d:	c3                   	ret    

0804859e <_Z14jump_function5v>:
 804859e:	55                   	push   %ebp
 804859f:	89 e5                	mov    %esp,%ebp
 80485a1:	e8 22 ff ff ff       	call   80484c8 <_Z9function5v>
 80485a6:	5d                   	pop    %ebp
 80485a7:	c3                   	ret    

080485a8 <_Z14jump_function6v>:
 80485a8:	55                   	push   %ebp
 80485a9:	89 e5                	mov    %esp,%ebp
 80485ab:	e8 35 ff ff ff       	call   80484e5 <_Z9function6v>
 80485b0:	5d                   	pop    %ebp
 80485b1:	c3                   	ret    

080485b2 <_Z14jump_function7v>:
 80485b2:	55                   	push   %ebp
 80485b3:	89 e5                	mov    %esp,%ebp
 80485b5:	e8 48 ff ff ff       	call   8048502 <_Z9function7v>
 80485ba:	5d                   	pop    %ebp
 80485bb:	c3                   	ret    

080485bc <_Z14jump_function8v>:
 80485bc:	55                   	push   %ebp
 80485bd:	89 e5                	mov    %esp,%ebp
 80485bf:	e8 5b ff ff ff       	call   804851f <_Z9function8v>
 80485c4:	5d                   	pop    %ebp
 80485c5:	c3                   	ret    

080485c6 <_Z14jump_function9v>:
 80485c6:	55                   	push   %ebp
 80485c7:	89 e5                	mov    %esp,%ebp
 80485c9:	e8 6e ff ff ff       	call   804853c <_Z9function9v>
 80485ce:	5d                   	pop    %ebp
 80485cf:	c3                   	ret    

080485d0 <_Z15jump_function10v>:
 80485d0:	55                   	push   %ebp
 80485d1:	89 e5                	mov    %esp,%ebp
 80485d3:	e8 81 ff ff ff       	call   8048559 <_Z10function10v>
 80485d8:	5d                   	pop    %ebp
 80485d9:	c3                   	ret    

080485da <main>:
 80485da:	55                   	push   %ebp
 80485db:	89 e5                	mov    %esp,%ebp
 80485dd:	83 e4 f0             	and    $0xfffffff0,%esp
 80485e0:	83 ec 50             	sub    $0x50,%esp
 80485e3:	e8 6c fe ff ff       	call   8048454 <_Z9function1v>
 80485e8:	e8 84 fe ff ff       	call   8048471 <_Z9function2v>
 80485ed:	e8 9c fe ff ff       	call   804848e <_Z9function3v>
 80485f2:	e8 b4 fe ff ff       	call   80484ab <_Z9function4v>
 80485f7:	e8 cc fe ff ff       	call   80484c8 <_Z9function5v>
 80485fc:	e8 e4 fe ff ff       	call   80484e5 <_Z9function6v>
 8048601:	e8 fc fe ff ff       	call   8048502 <_Z9function7v>
 8048606:	e8 14 ff ff ff       	call   804851f <_Z9function8v>
 804860b:	e8 2c ff ff ff       	call   804853c <_Z9function9v>
 8048610:	e8 44 ff ff ff       	call   8048559 <_Z10function10v>
 8048615:	e8 5c ff ff ff       	call   8048576 <_Z14jump_function1v>
 804861a:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 804861e:	e8 5d ff ff ff       	call   8048580 <_Z14jump_function2v>
 8048623:	89 44 24 20          	mov    %eax,0x20(%esp)
 8048627:	e8 5e ff ff ff       	call   804858a <_Z14jump_function3v>
 804862c:	89 44 24 24          	mov    %eax,0x24(%esp)
 8048630:	e8 5f ff ff ff       	call   8048594 <_Z14jump_function4v>
 8048635:	89 44 24 28          	mov    %eax,0x28(%esp)
 8048639:	e8 60 ff ff ff       	call   804859e <_Z14jump_function5v>
 804863e:	89 44 24 2c          	mov    %eax,0x2c(%esp)
 8048642:	e8 61 ff ff ff       	call   80485a8 <_Z14jump_function6v>
 8048647:	89 44 24 30          	mov    %eax,0x30(%esp)
 804864b:	e8 62 ff ff ff       	call   80485b2 <_Z14jump_function7v>
 8048650:	89 44 24 34          	mov    %eax,0x34(%esp)
 8048654:	e8 63 ff ff ff       	call   80485bc <_Z14jump_function8v>
 8048659:	89 44 24 38          	mov    %eax,0x38(%esp)
 804865d:	e8 64 ff ff ff       	call   80485c6 <_Z14jump_function9v>
 8048662:	89 44 24 3c          	mov    %eax,0x3c(%esp)
 8048666:	e8 65 ff ff ff       	call   80485d0 <_Z15jump_function10v>
 804866b:	89 44 24 40          	mov    %eax,0x40(%esp)
 804866f:	c7 44 24 44 54 84 04 	movl   $0x8048454,0x44(%esp)
 8048676:	08 
 8048677:	c7 44 24 4c 00 00 00 	movl   $0x0,0x4c(%esp)
 804867e:	00 
 804867f:	c7 44 24 48 00 00 00 	movl   $0x0,0x48(%esp)
 8048686:	00 
 8048687:	eb 31                	jmp    80486ba <main+0xe0>
 8048689:	8b 44 24 48          	mov    0x48(%esp),%eax
 804868d:	8b 44 84 1c          	mov    0x1c(%esp,%eax,4),%eax
 8048691:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048695:	8b 44 24 48          	mov    0x48(%esp),%eax
 8048699:	89 44 24 04          	mov    %eax,0x4(%esp)
 804869d:	c7 04 24 b8 87 04 08 	movl   $0x80487b8,(%esp)
 80486a4:	e8 c7 fc ff ff       	call   8048370 <printf@plt>
 80486a9:	8b 44 24 48          	mov    0x48(%esp),%eax
 80486ad:	8b 44 84 1c          	mov    0x1c(%esp,%eax,4),%eax
 80486b1:	01 44 24 4c          	add    %eax,0x4c(%esp)
 80486b5:	83 44 24 48 01       	addl   $0x1,0x48(%esp)
 80486ba:	83 7c 24 48 09       	cmpl   $0x9,0x48(%esp)
 80486bf:	0f 9e c0             	setle  %al
 80486c2:	84 c0                	test   %al,%al
 80486c4:	75 c3                	jne    8048689 <myhackedfunction>
 80486c6:	8b 44 24 4c          	mov    0x4c(%esp),%eax
 80486ca:	89 44 24 04          	mov    %eax,0x4(%esp)
 80486ce:	c7 04 24 c6 87 04 08 	movl   $0x80487c6,(%esp)
 80486d5:	e8 96 fc ff ff       	call   8048370 <printf@plt>
 80486da:	8b 44 24 44          	mov    0x44(%esp),%eax
 80486de:	ff d0                	call   *%eax
 80486e0:	c9                   	leave  
 80486e1:	c3                   	ret    
 80486e2:	90                   	nop    
 80486e3:	90                   	nop    
 80486e4:	90                   	nop    
 80486e5:	90                   	nop    
 80486e6:	90                   	nop    
 80486e7:	90                   	nop    
 80486e8:	90                   	nop    
 80486e9:	90                   	nop    
 80486ea:	90                   	nop    
 80486eb:	90                   	nop    
 80486ec:	90                   	nop    
 80486ed:	90                   	nop    
 80486ee:	90                   	nop    
 80486ef:	90                   	nop    

080486f0 <__libc_csu_init>:
 80486f0:	55                   	push   %ebp
 80486f1:	89 e5                	mov    %esp,%ebp
 80486f3:	57                   	push   %edi
 80486f4:	56                   	push   %esi
 80486f5:	53                   	push   %ebx
 80486f6:	e8 00 00 00 00       	call   80486fb <__libc_csu_init+0xb>
 80486fb:	5b                   	pop    %ebx
 80486fc:	81 c3 f9 18 00 00    	add    $0x18f9,%ebx
 8048702:	83 ec 1c             	sub    $0x1c,%esp
 8048705:	8d bb 00 ff ff ff    	lea    -0x100(%ebx),%edi
 804870b:	e8 18 fc ff ff       	call   8048328 <_init>
 8048710:	8d 83 00 ff ff ff    	lea    -0x100(%ebx),%eax
 8048716:	29 c7                	sub    %eax,%edi
 8048718:	c1 ff 02             	sar    $0x2,%edi
 804871b:	85 ff                	test   %edi,%edi
 804871d:	74 29                	je     8048748 <__libc_csu_init+0x58>
 804871f:	31 f6                	xor    %esi,%esi
 8048721:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8048728:	8b 45 10             	mov    0x10(%ebp),%eax
 804872b:	89 44 24 08          	mov    %eax,0x8(%esp)
 804872f:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048732:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048736:	8b 45 08             	mov    0x8(%ebp),%eax
 8048739:	89 04 24             	mov    %eax,(%esp)
 804873c:	ff 94 b3 00 ff ff ff 	call   *-0x100(%ebx,%esi,4)
 8048743:	46                   	inc    %esi
 8048744:	39 fe                	cmp    %edi,%esi
 8048746:	72 e0                	jb     8048728 <__libc_csu_init+0x38>
 8048748:	83 c4 1c             	add    $0x1c,%esp
 804874b:	5b                   	pop    %ebx
 804874c:	5e                   	pop    %esi
 804874d:	5f                   	pop    %edi
 804874e:	c9                   	leave  
 804874f:	c3                   	ret    

08048750 <__libc_csu_fini>:
 8048750:	55                   	push   %ebp
 8048751:	89 e5                	mov    %esp,%ebp
 8048753:	c9                   	leave  
 8048754:	c3                   	ret    
 8048755:	90                   	nop    
 8048756:	90                   	nop    
 8048757:	90                   	nop    
 8048758:	90                   	nop    
 8048759:	90                   	nop    
 804875a:	90                   	nop    
 804875b:	90                   	nop    
 804875c:	90                   	nop    
 804875d:	90                   	nop    
 804875e:	90                   	nop    
 804875f:	90                   	nop    
 8048760:	55                   	push   %ebp
 8048761:	89 e5                	mov    %esp,%ebp
 8048763:	53                   	push   %ebx
 8048764:	83 ec 04             	sub    $0x4,%esp
 8048767:	a1 f4 9e 04 08       	mov    0x8049ef4,%eax
 804876c:	83 f8 ff             	cmp    $0xffffffff,%eax
 804876f:	74 1b                	je     804878c <__libc_csu_fini+0x3c>
 8048771:	bb f4 9e 04 08       	mov    $0x8049ef4,%ebx
 8048776:	8d 76 00             	lea    0x0(%esi),%esi
 8048779:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
 8048780:	ff d0                	call   *%eax
 8048782:	83 eb 04             	sub    $0x4,%ebx
 8048785:	8b 03                	mov    (%ebx),%eax
 8048787:	83 f8 ff             	cmp    $0xffffffff,%eax
 804878a:	75 f4                	jne    8048780 <__libc_csu_fini+0x30>
 804878c:	83 c4 04             	add    $0x4,%esp
 804878f:	5b                   	pop    %ebx
 8048790:	c9                   	leave  
 8048791:	c3                   	ret    
 8048792:	90                   	nop    
 8048793:	90                   	nop    

Disassembly of section .fini:

08048794 <_fini>:
 8048794:	55                   	push   %ebp
 8048795:	89 e5                	mov    %esp,%ebp
 8048797:	53                   	push   %ebx
 8048798:	83 ec 04             	sub    $0x4,%esp
 804879b:	e8 00 00 00 00       	call   80487a0 <_fini+0xc>
 80487a0:	5b                   	pop    %ebx
 80487a1:	81 c3 54 18 00 00    	add    $0x1854,%ebx
 80487a7:	e8 24 fc ff ff       	call   80483d0 <call_gmon_start+0x2c>
 80487ac:	59                   	pop    %ecx
 80487ad:	5b                   	pop    %ebx
 80487ae:	c9                   	leave  
 80487af:	c3                   	ret    
