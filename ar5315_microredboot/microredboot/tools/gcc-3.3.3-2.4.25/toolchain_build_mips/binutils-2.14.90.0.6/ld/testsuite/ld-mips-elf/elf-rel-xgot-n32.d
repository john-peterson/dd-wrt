#name: MIPS ELF xgot reloc n32
#as: -n32 -KPIC -xgot
#source: ../../../gas/testsuite/gas/mips/elf-rel-got-n32.s
#ld: -melf32btsmipn32
#objdump: -D --show-raw-insn

.*: +file format elf32-n.*mips.*

Disassembly of section \.text:

100000a0 <fn>:
100000a0:	3c050000 	lui	a1,0x0
100000a4:	00bc2821 	addu	a1,a1,gp
100000a8:	8ca58034 	lw	a1,-32716\(a1\)
100000ac:	3c050000 	lui	a1,0x0
100000b0:	00bc2821 	addu	a1,a1,gp
100000b4:	8ca58034 	lw	a1,-32716\(a1\)
100000b8:	24a5000c 	addiu	a1,a1,12
100000bc:	3c050000 	lui	a1,0x0
100000c0:	00bc2821 	addu	a1,a1,gp
100000c4:	8ca58034 	lw	a1,-32716\(a1\)
100000c8:	3c010002 	lui	at,0x2
100000cc:	2421e240 	addiu	at,at,-7616
100000d0:	00a12821 	addu	a1,a1,at
100000d4:	3c050000 	lui	a1,0x0
100000d8:	00bc2821 	addu	a1,a1,gp
100000dc:	8ca58034 	lw	a1,-32716\(a1\)
100000e0:	00b12821 	addu	a1,a1,s1
100000e4:	3c050000 	lui	a1,0x0
100000e8:	00bc2821 	addu	a1,a1,gp
100000ec:	8ca58034 	lw	a1,-32716\(a1\)
100000f0:	24a5000c 	addiu	a1,a1,12
100000f4:	00b12821 	addu	a1,a1,s1
100000f8:	3c050000 	lui	a1,0x0
100000fc:	00bc2821 	addu	a1,a1,gp
10000100:	8ca58034 	lw	a1,-32716\(a1\)
10000104:	3c010002 	lui	at,0x2
10000108:	2421e240 	addiu	at,at,-7616
1000010c:	00a12821 	addu	a1,a1,at
10000110:	00b12821 	addu	a1,a1,s1
10000114:	3c050000 	lui	a1,0x0
10000118:	00bc2821 	addu	a1,a1,gp
1000011c:	8ca58034 	lw	a1,-32716\(a1\)
10000120:	8ca50000 	lw	a1,0\(a1\)
10000124:	3c050000 	lui	a1,0x0
10000128:	00bc2821 	addu	a1,a1,gp
1000012c:	8ca58034 	lw	a1,-32716\(a1\)
10000130:	8ca5000c 	lw	a1,12\(a1\)
10000134:	3c050000 	lui	a1,0x0
10000138:	00bc2821 	addu	a1,a1,gp
1000013c:	8ca58034 	lw	a1,-32716\(a1\)
10000140:	00b12821 	addu	a1,a1,s1
10000144:	8ca50000 	lw	a1,0\(a1\)
10000148:	3c050000 	lui	a1,0x0
1000014c:	00bc2821 	addu	a1,a1,gp
10000150:	8ca58034 	lw	a1,-32716\(a1\)
10000154:	00b12821 	addu	a1,a1,s1
10000158:	8ca5000c 	lw	a1,12\(a1\)
1000015c:	3c010000 	lui	at,0x0
10000160:	003c0821 	addu	at,at,gp
10000164:	8c218034 	lw	at,-32716\(at\)
10000168:	00250821 	addu	at,at,a1
1000016c:	8c250022 	lw	a1,34\(at\)
10000170:	3c010000 	lui	at,0x0
10000174:	003c0821 	addu	at,at,gp
10000178:	8c218034 	lw	at,-32716\(at\)
1000017c:	00250821 	addu	at,at,a1
10000180:	ac250038 	sw	a1,56\(at\)
10000184:	3c010000 	lui	at,0x0
10000188:	003c0821 	addu	at,at,gp
1000018c:	8c218034 	lw	at,-32716\(at\)
10000190:	88250000 	lwl	a1,0\(at\)
10000194:	98250003 	lwr	a1,3\(at\)
10000198:	3c010000 	lui	at,0x0
1000019c:	003c0821 	addu	at,at,gp
100001a0:	8c218034 	lw	at,-32716\(at\)
100001a4:	2421000c 	addiu	at,at,12
100001a8:	88250000 	lwl	a1,0\(at\)
100001ac:	98250003 	lwr	a1,3\(at\)
100001b0:	3c010000 	lui	at,0x0
100001b4:	003c0821 	addu	at,at,gp
100001b8:	8c218034 	lw	at,-32716\(at\)
100001bc:	00310821 	addu	at,at,s1
100001c0:	88250000 	lwl	a1,0\(at\)
100001c4:	98250003 	lwr	a1,3\(at\)
100001c8:	3c010000 	lui	at,0x0
100001cc:	003c0821 	addu	at,at,gp
100001d0:	8c218034 	lw	at,-32716\(at\)
100001d4:	2421000c 	addiu	at,at,12
100001d8:	00310821 	addu	at,at,s1
100001dc:	88250000 	lwl	a1,0\(at\)
100001e0:	98250003 	lwr	a1,3\(at\)
100001e4:	3c010000 	lui	at,0x0
100001e8:	003c0821 	addu	at,at,gp
100001ec:	8c218034 	lw	at,-32716\(at\)
100001f0:	24210022 	addiu	at,at,34
100001f4:	00250821 	addu	at,at,a1
100001f8:	88250000 	lwl	a1,0\(at\)
100001fc:	98250003 	lwr	a1,3\(at\)
10000200:	3c010000 	lui	at,0x0
10000204:	003c0821 	addu	at,at,gp
10000208:	8c218034 	lw	at,-32716\(at\)
1000020c:	24210038 	addiu	at,at,56
10000210:	00250821 	addu	at,at,a1
10000214:	a8250000 	swl	a1,0\(at\)
10000218:	b8250003 	swr	a1,3\(at\)
1000021c:	8f858018 	lw	a1,-32744\(gp\)
10000220:	24a506fc 	addiu	a1,a1,1788
10000224:	8f858018 	lw	a1,-32744\(gp\)
10000228:	24a50708 	addiu	a1,a1,1800
1000022c:	8f85801c 	lw	a1,-32740\(gp\)
10000230:	24a5e93c 	addiu	a1,a1,-5828
10000234:	8f858018 	lw	a1,-32744\(gp\)
10000238:	24a506fc 	addiu	a1,a1,1788
1000023c:	00b12821 	addu	a1,a1,s1
10000240:	8f858018 	lw	a1,-32744\(gp\)
10000244:	24a50708 	addiu	a1,a1,1800
10000248:	00b12821 	addu	a1,a1,s1
1000024c:	8f85801c 	lw	a1,-32740\(gp\)
10000250:	24a5e93c 	addiu	a1,a1,-5828
10000254:	00b12821 	addu	a1,a1,s1
10000258:	8f858018 	lw	a1,-32744\(gp\)
1000025c:	8ca506fc 	lw	a1,1788\(a1\)
10000260:	8f858018 	lw	a1,-32744\(gp\)
10000264:	8ca50708 	lw	a1,1800\(a1\)
10000268:	8f858018 	lw	a1,-32744\(gp\)
1000026c:	00b12821 	addu	a1,a1,s1
10000270:	8ca506fc 	lw	a1,1788\(a1\)
10000274:	8f858018 	lw	a1,-32744\(gp\)
10000278:	00b12821 	addu	a1,a1,s1
1000027c:	8ca50708 	lw	a1,1800\(a1\)
10000280:	8f818018 	lw	at,-32744\(gp\)
10000284:	00250821 	addu	at,at,a1
10000288:	8c25071e 	lw	a1,1822\(at\)
1000028c:	8f818018 	lw	at,-32744\(gp\)
10000290:	00250821 	addu	at,at,a1
10000294:	ac250734 	sw	a1,1844\(at\)
10000298:	8f818018 	lw	at,-32744\(gp\)
1000029c:	242106fc 	addiu	at,at,1788
100002a0:	88250000 	lwl	a1,0\(at\)
100002a4:	98250003 	lwr	a1,3\(at\)
100002a8:	8f818018 	lw	at,-32744\(gp\)
100002ac:	24210708 	addiu	at,at,1800
100002b0:	88250000 	lwl	a1,0\(at\)
100002b4:	98250003 	lwr	a1,3\(at\)
100002b8:	8f818018 	lw	at,-32744\(gp\)
100002bc:	242106fc 	addiu	at,at,1788
100002c0:	00310821 	addu	at,at,s1
100002c4:	88250000 	lwl	a1,0\(at\)
100002c8:	98250003 	lwr	a1,3\(at\)
100002cc:	8f818018 	lw	at,-32744\(gp\)
100002d0:	24210708 	addiu	at,at,1800
100002d4:	00310821 	addu	at,at,s1
100002d8:	88250000 	lwl	a1,0\(at\)
100002dc:	98250003 	lwr	a1,3\(at\)
100002e0:	8f818018 	lw	at,-32744\(gp\)
100002e4:	2421071e 	addiu	at,at,1822
100002e8:	00250821 	addu	at,at,a1
100002ec:	88250000 	lwl	a1,0\(at\)
100002f0:	98250003 	lwr	a1,3\(at\)
100002f4:	8f818018 	lw	at,-32744\(gp\)
100002f8:	24210734 	addiu	at,at,1844
100002fc:	00250821 	addu	at,at,a1
10000300:	a8250000 	swl	a1,0\(at\)
10000304:	b8250003 	swr	a1,3\(at\)
10000308:	3c050000 	lui	a1,0x0
1000030c:	00bc2821 	addu	a1,a1,gp
10000310:	8ca5802c 	lw	a1,-32724\(a1\)
10000314:	8f858020 	lw	a1,-32736\(gp\)
10000318:	24a500a0 	addiu	a1,a1,160
1000031c:	3c190000 	lui	t9,0x0
10000320:	033cc821 	addu	t9,t9,gp
10000324:	8f39802c 	lw	t9,-32724\(t9\)
10000328:	8f998020 	lw	t9,-32736\(gp\)
1000032c:	273900a0 	addiu	t9,t9,160
10000330:	3c190000 	lui	t9,0x0
10000334:	033cc821 	addu	t9,t9,gp
10000338:	8f39802c 	lw	t9,-32724\(t9\)
1000033c:	0320f809 	jalr	t9
10000340:	00000000 	nop
10000344:	8f998020 	lw	t9,-32736\(gp\)
10000348:	273900a0 	addiu	t9,t9,160
1000034c:	0320f809 	jalr	t9
10000350:	00000000 	nop
10000354:	3c050000 	lui	a1,0x0
10000358:	00bc2821 	addu	a1,a1,gp
1000035c:	8ca58038 	lw	a1,-32712\(a1\)
10000360:	3c050000 	lui	a1,0x0
10000364:	00bc2821 	addu	a1,a1,gp
10000368:	8ca58038 	lw	a1,-32712\(a1\)
1000036c:	24a5000c 	addiu	a1,a1,12
10000370:	3c050000 	lui	a1,0x0
10000374:	00bc2821 	addu	a1,a1,gp
10000378:	8ca58038 	lw	a1,-32712\(a1\)
1000037c:	3c010002 	lui	at,0x2
10000380:	2421e240 	addiu	at,at,-7616
10000384:	00a12821 	addu	a1,a1,at
10000388:	3c050000 	lui	a1,0x0
1000038c:	00bc2821 	addu	a1,a1,gp
10000390:	8ca58038 	lw	a1,-32712\(a1\)
10000394:	00b12821 	addu	a1,a1,s1
10000398:	3c050000 	lui	a1,0x0
1000039c:	00bc2821 	addu	a1,a1,gp
100003a0:	8ca58038 	lw	a1,-32712\(a1\)
100003a4:	24a5000c 	addiu	a1,a1,12
100003a8:	00b12821 	addu	a1,a1,s1
100003ac:	3c050000 	lui	a1,0x0
100003b0:	00bc2821 	addu	a1,a1,gp
100003b4:	8ca58038 	lw	a1,-32712\(a1\)
100003b8:	3c010002 	lui	at,0x2
100003bc:	2421e240 	addiu	at,at,-7616
100003c0:	00a12821 	addu	a1,a1,at
100003c4:	00b12821 	addu	a1,a1,s1
100003c8:	3c050000 	lui	a1,0x0
100003cc:	00bc2821 	addu	a1,a1,gp
100003d0:	8ca58038 	lw	a1,-32712\(a1\)
100003d4:	8ca50000 	lw	a1,0\(a1\)
100003d8:	3c050000 	lui	a1,0x0
100003dc:	00bc2821 	addu	a1,a1,gp
100003e0:	8ca58038 	lw	a1,-32712\(a1\)
100003e4:	8ca5000c 	lw	a1,12\(a1\)
100003e8:	3c050000 	lui	a1,0x0
100003ec:	00bc2821 	addu	a1,a1,gp
100003f0:	8ca58038 	lw	a1,-32712\(a1\)
100003f4:	00b12821 	addu	a1,a1,s1
100003f8:	8ca50000 	lw	a1,0\(a1\)
100003fc:	3c050000 	lui	a1,0x0
10000400:	00bc2821 	addu	a1,a1,gp
10000404:	8ca58038 	lw	a1,-32712\(a1\)
10000408:	00b12821 	addu	a1,a1,s1
1000040c:	8ca5000c 	lw	a1,12\(a1\)
10000410:	3c010000 	lui	at,0x0
10000414:	003c0821 	addu	at,at,gp
10000418:	8c218038 	lw	at,-32712\(at\)
1000041c:	00250821 	addu	at,at,a1
10000420:	8c250022 	lw	a1,34\(at\)
10000424:	3c010000 	lui	at,0x0
10000428:	003c0821 	addu	at,at,gp
1000042c:	8c218038 	lw	at,-32712\(at\)
10000430:	00250821 	addu	at,at,a1
10000434:	ac250038 	sw	a1,56\(at\)
10000438:	3c010000 	lui	at,0x0
1000043c:	003c0821 	addu	at,at,gp
10000440:	8c218038 	lw	at,-32712\(at\)
10000444:	88250000 	lwl	a1,0\(at\)
10000448:	98250003 	lwr	a1,3\(at\)
1000044c:	3c010000 	lui	at,0x0
10000450:	003c0821 	addu	at,at,gp
10000454:	8c218038 	lw	at,-32712\(at\)
10000458:	2421000c 	addiu	at,at,12
1000045c:	88250000 	lwl	a1,0\(at\)
10000460:	98250003 	lwr	a1,3\(at\)
10000464:	3c010000 	lui	at,0x0
10000468:	003c0821 	addu	at,at,gp
1000046c:	8c218038 	lw	at,-32712\(at\)
10000470:	00310821 	addu	at,at,s1
10000474:	88250000 	lwl	a1,0\(at\)
10000478:	98250003 	lwr	a1,3\(at\)
1000047c:	3c010000 	lui	at,0x0
10000480:	003c0821 	addu	at,at,gp
10000484:	8c218038 	lw	at,-32712\(at\)
10000488:	2421000c 	addiu	at,at,12
1000048c:	00310821 	addu	at,at,s1
10000490:	88250000 	lwl	a1,0\(at\)
10000494:	98250003 	lwr	a1,3\(at\)
10000498:	3c010000 	lui	at,0x0
1000049c:	003c0821 	addu	at,at,gp
100004a0:	8c218038 	lw	at,-32712\(at\)
100004a4:	24210022 	addiu	at,at,34
100004a8:	00250821 	addu	at,at,a1
100004ac:	88250000 	lwl	a1,0\(at\)
100004b0:	98250003 	lwr	a1,3\(at\)
100004b4:	3c010000 	lui	at,0x0
100004b8:	003c0821 	addu	at,at,gp
100004bc:	8c218038 	lw	at,-32712\(at\)
100004c0:	24210038 	addiu	at,at,56
100004c4:	00250821 	addu	at,at,a1
100004c8:	a8250000 	swl	a1,0\(at\)
100004cc:	b8250003 	swr	a1,3\(at\)
100004d0:	8f858018 	lw	a1,-32744\(gp\)
100004d4:	24a50774 	addiu	a1,a1,1908
100004d8:	8f858018 	lw	a1,-32744\(gp\)
100004dc:	24a50780 	addiu	a1,a1,1920
100004e0:	8f85801c 	lw	a1,-32740\(gp\)
100004e4:	24a5e9b4 	addiu	a1,a1,-5708
100004e8:	8f858018 	lw	a1,-32744\(gp\)
100004ec:	24a50774 	addiu	a1,a1,1908
100004f0:	00b12821 	addu	a1,a1,s1
100004f4:	8f858018 	lw	a1,-32744\(gp\)
100004f8:	24a50780 	addiu	a1,a1,1920
100004fc:	00b12821 	addu	a1,a1,s1
10000500:	8f85801c 	lw	a1,-32740\(gp\)
10000504:	24a5e9b4 	addiu	a1,a1,-5708
10000508:	00b12821 	addu	a1,a1,s1
1000050c:	8f858018 	lw	a1,-32744\(gp\)
10000510:	8ca50774 	lw	a1,1908\(a1\)
10000514:	8f858018 	lw	a1,-32744\(gp\)
10000518:	8ca50780 	lw	a1,1920\(a1\)
1000051c:	8f858018 	lw	a1,-32744\(gp\)
10000520:	00b12821 	addu	a1,a1,s1
10000524:	8ca50774 	lw	a1,1908\(a1\)
10000528:	8f858018 	lw	a1,-32744\(gp\)
1000052c:	00b12821 	addu	a1,a1,s1
10000530:	8ca50780 	lw	a1,1920\(a1\)
10000534:	8f818018 	lw	at,-32744\(gp\)
10000538:	00250821 	addu	at,at,a1
1000053c:	8c250774 	lw	a1,1908\(at\)
10000540:	8f818018 	lw	at,-32744\(gp\)
10000544:	00250821 	addu	at,at,a1
10000548:	ac2507ac 	sw	a1,1964\(at\)
1000054c:	8f818018 	lw	at,-32744\(gp\)
10000550:	24210774 	addiu	at,at,1908
10000554:	88250000 	lwl	a1,0\(at\)
10000558:	98250003 	lwr	a1,3\(at\)
1000055c:	8f818018 	lw	at,-32744\(gp\)
10000560:	24210780 	addiu	at,at,1920
10000564:	88250000 	lwl	a1,0\(at\)
10000568:	98250003 	lwr	a1,3\(at\)
1000056c:	8f818018 	lw	at,-32744\(gp\)
10000570:	24210774 	addiu	at,at,1908
10000574:	00310821 	addu	at,at,s1
10000578:	88250000 	lwl	a1,0\(at\)
1000057c:	98250003 	lwr	a1,3\(at\)
10000580:	8f818018 	lw	at,-32744\(gp\)
10000584:	24210780 	addiu	at,at,1920
10000588:	00310821 	addu	at,at,s1
1000058c:	88250000 	lwl	a1,0\(at\)
10000590:	98250003 	lwr	a1,3\(at\)
10000594:	8f818018 	lw	at,-32744\(gp\)
10000598:	24210796 	addiu	at,at,1942
1000059c:	00250821 	addu	at,at,a1
100005a0:	88250000 	lwl	a1,0\(at\)
100005a4:	98250003 	lwr	a1,3\(at\)
100005a8:	8f818018 	lw	at,-32744\(gp\)
100005ac:	242107ac 	addiu	at,at,1964
100005b0:	00250821 	addu	at,at,a1
100005b4:	a8250000 	swl	a1,0\(at\)
100005b8:	b8250003 	swr	a1,3\(at\)
100005bc:	3c050000 	lui	a1,0x0
100005c0:	00bc2821 	addu	a1,a1,gp
100005c4:	8ca58030 	lw	a1,-32720\(a1\)
100005c8:	8f858020 	lw	a1,-32736\(gp\)
100005cc:	24a506a0 	addiu	a1,a1,1696
100005d0:	3c190000 	lui	t9,0x0
100005d4:	033cc821 	addu	t9,t9,gp
100005d8:	8f398030 	lw	t9,-32720\(t9\)
100005dc:	8f998020 	lw	t9,-32736\(gp\)
100005e0:	273906a0 	addiu	t9,t9,1696
100005e4:	3c190000 	lui	t9,0x0
100005e8:	033cc821 	addu	t9,t9,gp
100005ec:	8f398030 	lw	t9,-32720\(t9\)
100005f0:	0320f809 	jalr	t9
100005f4:	00000000 	nop
100005f8:	8f998020 	lw	t9,-32736\(gp\)
100005fc:	273906a0 	addiu	t9,t9,1696
10000600:	0320f809 	jalr	t9
10000604:	00000000 	nop
10000608:	3c050000 	lui	a1,0x0
1000060c:	00bc2821 	addu	a1,a1,gp
10000610:	8ca58034 	lw	a1,-32716\(a1\)
10000614:	1000fea2 	b	100000a0 <fn>
10000618:	00000000 	nop
1000061c:	3c050000 	lui	a1,0x0
10000620:	00bc2821 	addu	a1,a1,gp
10000624:	8ca58038 	lw	a1,-32712\(a1\)
10000628:	8ca50000 	lw	a1,0\(a1\)
1000062c:	1000001c 	b	100006a0 <fn2>
10000630:	00000000 	nop
10000634:	8f858018 	lw	a1,-32744\(gp\)
10000638:	24a506fc 	addiu	a1,a1,1788
1000063c:	1000fe98 	b	100000a0 <fn>
10000640:	00000000 	nop
10000644:	8f858018 	lw	a1,-32744\(gp\)
10000648:	24a50780 	addiu	a1,a1,1920
1000064c:	10000014 	b	100006a0 <fn2>
10000650:	00000000 	nop
10000654:	8f85801c 	lw	a1,-32740\(gp\)
10000658:	24a5e93c 	addiu	a1,a1,-5828
1000065c:	1000fe90 	b	100000a0 <fn>
10000660:	00000000 	nop
10000664:	8f858018 	lw	a1,-32744\(gp\)
10000668:	8ca50774 	lw	a1,1908\(a1\)
1000066c:	1000000c 	b	100006a0 <fn2>
10000670:	00000000 	nop
10000674:	8f858018 	lw	a1,-32744\(gp\)
10000678:	8ca50708 	lw	a1,1800\(a1\)
1000067c:	1000fe88 	b	100000a0 <fn>
10000680:	00000000 	nop
10000684:	8f818018 	lw	at,-32744\(gp\)
10000688:	00250821 	addu	at,at,a1
1000068c:	8c250796 	lw	a1,1942\(at\)
10000690:	10000003 	b	100006a0 <fn2>
10000694:	00000000 	nop
	\.\.\.
Disassembly of section \.reginfo:

100006a0 <\.reginfo>:
100006a0:	92020022 	.*
	\.\.\.
100006b4:	101087a0 	.*
Disassembly of section \.data:

101006c0 <_fdata>:
	\.\.\.

101006fc <dg1>:
	\.\.\.

10100738 <sp2>:
	\.\.\.

10100774 <dg2>:
	\.\.\.
Disassembly of section \.got:

101007b0 <_GLOBAL_OFFSET_TABLE_>:
101007b0:	00000000 	.*
101007b4:	80000000 	.*
101007b8:	10100000 	.*
101007bc:	10120000 	.*
101007c0:	10000000 	.*
101007c4:	00000000 	.*
101007c8:	00000000 	.*
101007cc:	100000a0 	.*
101007d0:	100006a0 	.*
101007d4:	101006fc 	.*
101007d8:	10100774 	.*
