#name: MIPS ELF got reloc n64
#as: -64 -KPIC
#source: ../../../gas/testsuite/gas/mips/elf-rel-got-n64.s
#ld: -melf64btsmip
#objdump: -D --show-raw-insn

.*: +file format elf64-.*mips.*

Disassembly of section \.MIPS\.options:

00000001200000b0 <\.MIPS\.options>:
   1200000b0:	01280000 	.*
   1200000b4:	00000000 	.*
   1200000b8:	92020022 	.*
	\.\.\.
   1200000d0:	00000001 	.*
   1200000d4:	201085d0 	.*
Disassembly of section \.text:

00000001200000e0 <fn>:
   1200000e0:	df8580b8 	ld	a1,-32584\(gp\)
   1200000e4:	df8580b8 	ld	a1,-32584\(gp\)
   1200000e8:	64a5000c 	daddiu	a1,a1,12
   1200000ec:	df8580b8 	ld	a1,-32584\(gp\)
   1200000f0:	3c010002 	lui	at,0x2
   1200000f4:	6421e240 	daddiu	at,at,-7616
   1200000f8:	00a1282d 	daddu	a1,a1,at
   1200000fc:	df8580b8 	ld	a1,-32584\(gp\)
   120000100:	00b1282d 	daddu	a1,a1,s1
   120000104:	df8580b8 	ld	a1,-32584\(gp\)
   120000108:	64a5000c 	daddiu	a1,a1,12
   12000010c:	00b1282d 	daddu	a1,a1,s1
   120000110:	df8580b8 	ld	a1,-32584\(gp\)
   120000114:	3c010002 	lui	at,0x2
   120000118:	6421e240 	daddiu	at,at,-7616
   12000011c:	00a1282d 	daddu	a1,a1,at
   120000120:	00b1282d 	daddu	a1,a1,s1
   120000124:	df8580b8 	ld	a1,-32584\(gp\)
   120000128:	dca50000 	ld	a1,0\(a1\)
   12000012c:	df8580b8 	ld	a1,-32584\(gp\)
   120000130:	dca5000c 	ld	a1,12\(a1\)
   120000134:	df8580b8 	ld	a1,-32584\(gp\)
   120000138:	00b1282d 	daddu	a1,a1,s1
   12000013c:	dca50000 	ld	a1,0\(a1\)
   120000140:	df8580b8 	ld	a1,-32584\(gp\)
   120000144:	00b1282d 	daddu	a1,a1,s1
   120000148:	dca5000c 	ld	a1,12\(a1\)
   12000014c:	df8180b8 	ld	at,-32584\(gp\)
   120000150:	0025082d 	daddu	at,at,a1
   120000154:	dc250022 	ld	a1,34\(at\)
   120000158:	df8180b8 	ld	at,-32584\(gp\)
   12000015c:	0025082d 	daddu	at,at,a1
   120000160:	fc250038 	sd	a1,56\(at\)
   120000164:	df8180b8 	ld	at,-32584\(gp\)
   120000168:	88250000 	lwl	a1,0\(at\)
   12000016c:	98250003 	lwr	a1,3\(at\)
   120000170:	df8180b8 	ld	at,-32584\(gp\)
   120000174:	6421000c 	daddiu	at,at,12
   120000178:	88250000 	lwl	a1,0\(at\)
   12000017c:	98250003 	lwr	a1,3\(at\)
   120000180:	df8180b8 	ld	at,-32584\(gp\)
   120000184:	0031082d 	daddu	at,at,s1
   120000188:	88250000 	lwl	a1,0\(at\)
   12000018c:	98250003 	lwr	a1,3\(at\)
   120000190:	df8180b8 	ld	at,-32584\(gp\)
   120000194:	6421000c 	daddiu	at,at,12
   120000198:	0031082d 	daddu	at,at,s1
   12000019c:	88250000 	lwl	a1,0\(at\)
   1200001a0:	98250003 	lwr	a1,3\(at\)
   1200001a4:	df8180b8 	ld	at,-32584\(gp\)
   1200001a8:	64210022 	daddiu	at,at,34
   1200001ac:	0025082d 	daddu	at,at,a1
   1200001b0:	88250000 	lwl	a1,0\(at\)
   1200001b4:	98250003 	lwr	a1,3\(at\)
   1200001b8:	df8180b8 	ld	at,-32584\(gp\)
   1200001bc:	64210038 	daddiu	at,at,56
   1200001c0:	0025082d 	daddu	at,at,a1
   1200001c4:	a8250000 	swl	a1,0\(at\)
   1200001c8:	b8250003 	swr	a1,3\(at\)
   1200001cc:	df858020 	ld	a1,-32736\(gp\)
   1200001d0:	df858028 	ld	a1,-32728\(gp\)
   1200001d4:	df858030 	ld	a1,-32720\(gp\)
   1200001d8:	df858020 	ld	a1,-32736\(gp\)
   1200001dc:	00b1282d 	daddu	a1,a1,s1
   1200001e0:	df858028 	ld	a1,-32728\(gp\)
   1200001e4:	00b1282d 	daddu	a1,a1,s1
   1200001e8:	df858030 	ld	a1,-32720\(gp\)
   1200001ec:	00b1282d 	daddu	a1,a1,s1
   1200001f0:	df858038 	ld	a1,-32712\(gp\)
   1200001f4:	dca5052c 	ld	a1,1324\(a1\)
   1200001f8:	df858038 	ld	a1,-32712\(gp\)
   1200001fc:	dca50538 	ld	a1,1336\(a1\)
   120000200:	df858038 	ld	a1,-32712\(gp\)
   120000204:	00b1282d 	daddu	a1,a1,s1
   120000208:	dca5052c 	ld	a1,1324\(a1\)
   12000020c:	df858038 	ld	a1,-32712\(gp\)
   120000210:	00b1282d 	daddu	a1,a1,s1
   120000214:	dca50538 	ld	a1,1336\(a1\)
   120000218:	df818038 	ld	at,-32712\(gp\)
   12000021c:	0025082d 	daddu	at,at,a1
   120000220:	dc25054e 	ld	a1,1358\(at\)
   120000224:	df818038 	ld	at,-32712\(gp\)
   120000228:	0025082d 	daddu	at,at,a1
   12000022c:	fc250564 	sd	a1,1380\(at\)
   120000230:	df818020 	ld	at,-32736\(gp\)
   120000234:	88250000 	lwl	a1,0\(at\)
   120000238:	98250003 	lwr	a1,3\(at\)
   12000023c:	df818028 	ld	at,-32728\(gp\)
   120000240:	88250000 	lwl	a1,0\(at\)
   120000244:	98250003 	lwr	a1,3\(at\)
   120000248:	df818020 	ld	at,-32736\(gp\)
   12000024c:	0031082d 	daddu	at,at,s1
   120000250:	88250000 	lwl	a1,0\(at\)
   120000254:	98250003 	lwr	a1,3\(at\)
   120000258:	df818028 	ld	at,-32728\(gp\)
   12000025c:	0031082d 	daddu	at,at,s1
   120000260:	88250000 	lwl	a1,0\(at\)
   120000264:	98250003 	lwr	a1,3\(at\)
   120000268:	df818040 	ld	at,-32704\(gp\)
   12000026c:	0025082d 	daddu	at,at,a1
   120000270:	88250000 	lwl	a1,0\(at\)
   120000274:	98250003 	lwr	a1,3\(at\)
   120000278:	df818048 	ld	at,-32696\(gp\)
   12000027c:	0025082d 	daddu	at,at,a1
   120000280:	a8250000 	swl	a1,0\(at\)
   120000284:	b8250003 	swr	a1,3\(at\)
   120000288:	df8580a8 	ld	a1,-32600\(gp\)
   12000028c:	df858050 	ld	a1,-32688\(gp\)
   120000290:	df9980a8 	ld	t9,-32600\(gp\)
   120000294:	df998050 	ld	t9,-32688\(gp\)
   120000298:	df9980a8 	ld	t9,-32600\(gp\)
   12000029c:	0320f809 	jalr	t9
   1200002a0:	00000000 	nop
   1200002a4:	df998050 	ld	t9,-32688\(gp\)
   1200002a8:	0320f809 	jalr	t9
   1200002ac:	00000000 	nop
   1200002b0:	df8580c0 	ld	a1,-32576\(gp\)
   1200002b4:	df8580c0 	ld	a1,-32576\(gp\)
   1200002b8:	64a5000c 	daddiu	a1,a1,12
   1200002bc:	df8580c0 	ld	a1,-32576\(gp\)
   1200002c0:	3c010002 	lui	at,0x2
   1200002c4:	6421e240 	daddiu	at,at,-7616
   1200002c8:	00a1282d 	daddu	a1,a1,at
   1200002cc:	df8580c0 	ld	a1,-32576\(gp\)
   1200002d0:	00b1282d 	daddu	a1,a1,s1
   1200002d4:	df8580c0 	ld	a1,-32576\(gp\)
   1200002d8:	64a5000c 	daddiu	a1,a1,12
   1200002dc:	00b1282d 	daddu	a1,a1,s1
   1200002e0:	df8580c0 	ld	a1,-32576\(gp\)
   1200002e4:	3c010002 	lui	at,0x2
   1200002e8:	6421e240 	daddiu	at,at,-7616
   1200002ec:	00a1282d 	daddu	a1,a1,at
   1200002f0:	00b1282d 	daddu	a1,a1,s1
   1200002f4:	df8580c0 	ld	a1,-32576\(gp\)
   1200002f8:	dca50000 	ld	a1,0\(a1\)
   1200002fc:	df8580c0 	ld	a1,-32576\(gp\)
   120000300:	dca5000c 	ld	a1,12\(a1\)
   120000304:	df8580c0 	ld	a1,-32576\(gp\)
   120000308:	00b1282d 	daddu	a1,a1,s1
   12000030c:	dca50000 	ld	a1,0\(a1\)
   120000310:	df8580c0 	ld	a1,-32576\(gp\)
   120000314:	00b1282d 	daddu	a1,a1,s1
   120000318:	dca5000c 	ld	a1,12\(a1\)
   12000031c:	df8180c0 	ld	at,-32576\(gp\)
   120000320:	0025082d 	daddu	at,at,a1
   120000324:	dc250022 	ld	a1,34\(at\)
   120000328:	df8180c0 	ld	at,-32576\(gp\)
   12000032c:	0025082d 	daddu	at,at,a1
   120000330:	fc250038 	sd	a1,56\(at\)
   120000334:	df8180c0 	ld	at,-32576\(gp\)
   120000338:	88250000 	lwl	a1,0\(at\)
   12000033c:	98250003 	lwr	a1,3\(at\)
   120000340:	df8180c0 	ld	at,-32576\(gp\)
   120000344:	6421000c 	daddiu	at,at,12
   120000348:	88250000 	lwl	a1,0\(at\)
   12000034c:	98250003 	lwr	a1,3\(at\)
   120000350:	df8180c0 	ld	at,-32576\(gp\)
   120000354:	0031082d 	daddu	at,at,s1
   120000358:	88250000 	lwl	a1,0\(at\)
   12000035c:	98250003 	lwr	a1,3\(at\)
   120000360:	df8180c0 	ld	at,-32576\(gp\)
   120000364:	6421000c 	daddiu	at,at,12
   120000368:	0031082d 	daddu	at,at,s1
   12000036c:	88250000 	lwl	a1,0\(at\)
   120000370:	98250003 	lwr	a1,3\(at\)
   120000374:	df8180c0 	ld	at,-32576\(gp\)
   120000378:	64210022 	daddiu	at,at,34
   12000037c:	0025082d 	daddu	at,at,a1
   120000380:	88250000 	lwl	a1,0\(at\)
   120000384:	98250003 	lwr	a1,3\(at\)
   120000388:	df8180c0 	ld	at,-32576\(gp\)
   12000038c:	64210038 	daddiu	at,at,56
   120000390:	0025082d 	daddu	at,at,a1
   120000394:	a8250000 	swl	a1,0\(at\)
   120000398:	b8250003 	swr	a1,3\(at\)
   12000039c:	df858058 	ld	a1,-32680\(gp\)
   1200003a0:	df858060 	ld	a1,-32672\(gp\)
   1200003a4:	df858068 	ld	a1,-32664\(gp\)
   1200003a8:	df858058 	ld	a1,-32680\(gp\)
   1200003ac:	00b1282d 	daddu	a1,a1,s1
   1200003b0:	df858060 	ld	a1,-32672\(gp\)
   1200003b4:	00b1282d 	daddu	a1,a1,s1
   1200003b8:	df858068 	ld	a1,-32664\(gp\)
   1200003bc:	00b1282d 	daddu	a1,a1,s1
   1200003c0:	df858038 	ld	a1,-32712\(gp\)
   1200003c4:	dca505a4 	ld	a1,1444\(a1\)
   1200003c8:	df858038 	ld	a1,-32712\(gp\)
   1200003cc:	dca505b0 	ld	a1,1456\(a1\)
   1200003d0:	df858038 	ld	a1,-32712\(gp\)
   1200003d4:	00b1282d 	daddu	a1,a1,s1
   1200003d8:	dca505a4 	ld	a1,1444\(a1\)
   1200003dc:	df858038 	ld	a1,-32712\(gp\)
   1200003e0:	00b1282d 	daddu	a1,a1,s1
   1200003e4:	dca505b0 	ld	a1,1456\(a1\)
   1200003e8:	df818038 	ld	at,-32712\(gp\)
   1200003ec:	0025082d 	daddu	at,at,a1
   1200003f0:	dc2505c6 	ld	a1,1478\(at\)
   1200003f4:	df818038 	ld	at,-32712\(gp\)
   1200003f8:	0025082d 	daddu	at,at,a1
   1200003fc:	fc2505dc 	sd	a1,1500\(at\)
   120000400:	df818058 	ld	at,-32680\(gp\)
   120000404:	88250000 	lwl	a1,0\(at\)
   120000408:	98250003 	lwr	a1,3\(at\)
   12000040c:	df818060 	ld	at,-32672\(gp\)
   120000410:	88250000 	lwl	a1,0\(at\)
   120000414:	98250003 	lwr	a1,3\(at\)
   120000418:	df818058 	ld	at,-32680\(gp\)
   12000041c:	0031082d 	daddu	at,at,s1
   120000420:	88250000 	lwl	a1,0\(at\)
   120000424:	98250003 	lwr	a1,3\(at\)
   120000428:	df818060 	ld	at,-32672\(gp\)
   12000042c:	0031082d 	daddu	at,at,s1
   120000430:	88250000 	lwl	a1,0\(at\)
   120000434:	98250003 	lwr	a1,3\(at\)
   120000438:	df818070 	ld	at,-32656\(gp\)
   12000043c:	0025082d 	daddu	at,at,a1
   120000440:	88250000 	lwl	a1,0\(at\)
   120000444:	98250003 	lwr	a1,3\(at\)
   120000448:	df818078 	ld	at,-32648\(gp\)
   12000044c:	0025082d 	daddu	at,at,a1
   120000450:	a8250000 	swl	a1,0\(at\)
   120000454:	b8250003 	swr	a1,3\(at\)
   120000458:	df8580b0 	ld	a1,-32592\(gp\)
   12000045c:	df858080 	ld	a1,-32640\(gp\)
   120000460:	df9980b0 	ld	t9,-32592\(gp\)
   120000464:	df998080 	ld	t9,-32640\(gp\)
   120000468:	df9980b0 	ld	t9,-32592\(gp\)
   12000046c:	0320f809 	jalr	t9
   120000470:	00000000 	nop
   120000474:	df998080 	ld	t9,-32640\(gp\)
   120000478:	0320f809 	jalr	t9
   12000047c:	00000000 	nop
   120000480:	1000ff17 	b	1200000e0 <fn>
   120000484:	df8580b8 	ld	a1,-32584\(gp\)
   120000488:	df8580c0 	ld	a1,-32576\(gp\)
   12000048c:	10000015 	b	1200004e4 <fn2>
   120000490:	dca50000 	ld	a1,0\(a1\)
   120000494:	1000ff12 	b	1200000e0 <fn>
   120000498:	df858020 	ld	a1,-32736\(gp\)
   12000049c:	df858060 	ld	a1,-32672\(gp\)
   1200004a0:	10000010 	b	1200004e4 <fn2>
   1200004a4:	00000000 	nop
   1200004a8:	df858030 	ld	a1,-32720\(gp\)
   1200004ac:	1000ff0c 	b	1200000e0 <fn>
   1200004b0:	00000000 	nop
   1200004b4:	df858038 	ld	a1,-32712\(gp\)
   1200004b8:	1000000a 	b	1200004e4 <fn2>
   1200004bc:	dca505a4 	ld	a1,1444\(a1\)
   1200004c0:	df858038 	ld	a1,-32712\(gp\)
   1200004c4:	1000ff06 	b	1200000e0 <fn>
   1200004c8:	dca50538 	ld	a1,1336\(a1\)
   1200004cc:	df818038 	ld	at,-32712\(gp\)
   1200004d0:	0025082d 	daddu	at,at,a1
   1200004d4:	10000003 	b	1200004e4 <fn2>
   1200004d8:	dc2505c6 	ld	a1,1478\(at\)
	\.\.\.

00000001200004e4 <fn2>:
	\.\.\.
Disassembly of section \.data:

00000001201004f0 <_fdata>:
	\.\.\.

000000012010052c <dg1>:
	\.\.\.

0000000120100568 <sp2>:
	\.\.\.

00000001201005a4 <dg2>:
	\.\.\.
Disassembly of section \.got:

00000001201005e0 <_GLOBAL_OFFSET_TABLE_>:
	\.\.\.
   1201005ec:	80000000 	.*
   1201005f0:	00000001 	.*
   1201005f4:	2010052c 	.*
   1201005f8:	00000001 	.*
   1201005fc:	20100538 	.*
   120100600:	00000001 	.*
   120100604:	2011e76c 	.*
   120100608:	00000001 	.*
   12010060c:	20100000 	.*
   120100610:	00000001 	.*
   120100614:	2010054e 	.*
   120100618:	00000001 	.*
   12010061c:	20100564 	.*
   120100620:	00000001 	.*
   120100624:	200000e0 	.*
   120100628:	00000001 	.*
   12010062c:	201005a4 	.*
   120100630:	00000001 	.*
   120100634:	201005b0 	.*
   120100638:	00000001 	.*
   12010063c:	2011e7e4 	.*
   120100640:	00000001 	.*
   120100644:	201005c6 	.*
   120100648:	00000001 	.*
   12010064c:	201005dc 	.*
   120100650:	00000001 	.*
   120100654:	200004e4 	.*
	\.\.\.
   120100678:	00000001 	.*
   12010067c:	200000e0 	.*
   120100680:	00000001 	.*
   120100684:	200004e4 	.*
   120100688:	00000001 	.*
   12010068c:	2010052c 	.*
   120100690:	00000001 	.*
   120100694:	201005a4 	.*
