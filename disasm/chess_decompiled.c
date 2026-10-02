// ---- function FUN_1000_0000 at 1000:0000 ----

void __cdecl16near FUN_1000_0000(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined2 unaff_DS;
  
  puVar1 = (undefined2 *)*(undefined2 *)0x9cc;
  if (puVar1 < (undefined2 *)0x9aa) {
    puVar2 = (undefined2 *)*(undefined2 *)0x9ca;
    *(undefined2 *)0x9ca = puVar1;
    *puVar2 = puVar1;
    if ((*(byte *)0x9d0 & 8) == 0) {
      *(byte *)0x9d1 = *(byte *)0x9d1 | 0x10;
    }
    *puVar1 = 0;
    *(undefined1 *)(puVar1 + 1) = *(undefined1 *)0x9b2;
    *(undefined1 *)((int)puVar1 + 3) = *(undefined1 *)0x9b4;
    *(undefined1 *)(puVar1 + 2) = *(undefined1 *)0x9d1;
    *(undefined1 *)((int)puVar1 + 5) = 0;
    *(undefined2 *)0x9cc = puVar1 + 3;
    return;
  }
  *(undefined2 *)0x9aa = 0;
  return;
}



// ---- function FUN_1000_004c at 1000:004c ----

void __cdecl16near FUN_1000_004c(void)

{
  byte *pbVar1;
  int iVar2;
  undefined2 *puVar3;
  undefined2 unaff_DS;
  
  pbVar1 = (byte *)0x9cf;
  *pbVar1 = *pbVar1 ^ 0x80;
  if ((char)*pbVar1 < '\0') {
    *(char *)0x9d4 = *(char *)0x9d4 + -1;
  }
  *(int *)0x9c6 = *(int *)0x9c6 + -1;
  *(char *)0x9d6 = *(char *)0x9d6 + -1;
  iVar2 = *(int *)0x9c2;
  puVar3 = (undefined2 *)(iVar2 + -4);
  *(undefined2 *)0x9c2 = puVar3;
  *(undefined2 *)0x9cc = *(undefined2 *)(iVar2 + -2);
  *(undefined2 *)0x9c4 = *puVar3;
  FUN_1000_0ec2();
  return;
}



// ---- function FUN_1000_007a at 1000:007a ----

void __cdecl16near FUN_1000_007a(void)

{
  char *pcVar1;
  int iVar2;
  undefined2 uVar3;
  char in_CL;
  uint uVar4;
  uint in_DX;
  char *pcVar5;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  bool bVar6;
  
  bVar6 = *(byte *)0x9c0 == 0;
  if (!bVar6) {
    in_DX = in_DX & 0xff00;
    uVar4 = (uint)*(byte *)0x9c0;
    pcVar5 = (char *)0x165;
LAB_1000_0094:
    do {
      if (uVar4 != 0) {
        uVar4 = uVar4 - 1;
        pcVar1 = pcVar5;
        pcVar5 = pcVar5 + 1;
        bVar6 = *(char *)0x9b4 == *pcVar1;
        if (!bVar6) goto LAB_1000_0094;
      }
      if (!bVar6) break;
      if ((char)in_DX != '\0') {
        return;
      }
      in_DX = CONCAT11((char)(in_DX >> 8),1);
      bVar6 = true;
      if ((pcVar5[9] != in_CL) && (bVar6 = true, -pcVar5[9] != in_CL)) {
        return;
      }
    } while( true );
  }
  iVar2 = *(int *)0x9bc;
  pcVar5 = (char *)0x149;
  if ((*(byte *)0x9d1 & 0x80) != 0) {
    pcVar5 = (char *)0x157;
  }
  uVar4 = *(byte *)0x9d1 & 0xff07;
  if ((in_DX & 0x8000) != 0) {
    uVar4 = 5;
  }
  *pcVar5 = *pcVar5 + '\x01';
  pcVar5 = pcVar5 + uVar4 * 2;
  uVar3 = *(undefined2 *)pcVar5;
  if ((char)uVar3 != '\0') {
    if ((char)((uint)uVar3 >> 8) == '\0') {
      uVar3 = CONCAT11(*(undefined1 *)(iVar2 + 0xc2),(char)uVar3);
      goto LAB_1000_00f1;
    }
    pcVar5 = pcVar5 + 2;
    uVar3 = *(undefined2 *)pcVar5;
  }
  uVar3 = CONCAT11((char)uVar3,*(undefined1 *)(iVar2 + 0xc2));
LAB_1000_00f1:
  *(undefined2 *)pcVar5 = uVar3;
  return;
}



// ---- function FUN_1000_00f8 at 1000:00f8 ----

undefined1 __cdecl16near FUN_1000_00f8(void)

{
  char cVar1;
  int iVar2;
  byte bVar3;
  byte extraout_DH;
  byte bVar4;
  undefined2 unaff_DS;
  
  iVar2 = 0x1000;
  do {
    *(undefined1 *)0x9b4 = *(undefined1 *)0x9b6;
    while( true ) {
      while( true ) {
        cVar1 = FUN_1000_0af6();
        bVar3 = (byte)((uint)iVar2 >> 8);
        if (cVar1 == '\x01') break;
        if (cVar1 == '\x02') {
          bVar4 = extraout_DH;
          if ((extraout_DH & 0x20) == 0) goto LAB_1000_013c;
          goto LAB_1000_0120;
        }
        if ((cVar1 != '\0') || (bVar3 < 9)) goto LAB_1000_0120;
      }
      if ((extraout_DH & 0x40) != 0) break;
      bVar4 = extraout_DH | 0x20;
LAB_1000_013c:
      cVar1 = *(char *)0x9bc;
      if (bVar3 < 9) {
        if (cVar1 != '\x02') break;
      }
      else if ((cVar1 != '\x05') && (((bVar4 & 0xf) != 1 || (cVar1 != '\x06')))) {
        if (bVar3 < 0xd) {
          if (cVar1 != '\x04') break;
        }
        else if (cVar1 != '\x03') {
          if (((bVar4 & 0xf) != 1) || (cVar1 != '\x01')) break;
          if ((*(byte *)0x9d1 & 0x80) == 0) {
            if (0xe < bVar3) break;
          }
          else if (bVar3 < 0xf) break;
        }
      }
      if (*(char *)0x9ba == '\a') {
        FUN_1000_007a();
      }
      else if ((bVar4 & 0x20) != 0) {
        return 1;
      }
      bVar3 = (byte)((uint)iVar2 >> 8);
      if ((*(char *)0x9bc == '\x06') || (*(char *)0x9bc == '\x02')) break;
    }
LAB_1000_0120:
    iVar2 = (uint)(byte)(bVar3 - 1) << 8;
    if ((byte)(bVar3 - 1) == 0) {
      return 0;
    }
  } while( true );
}



// ---- function FUN_1000_01ba at 1000:01ba ----

undefined2 __cdecl16near FUN_1000_01ba(void)

{
  byte in_AL;
  
  return CONCAT11(in_AL / 10 + 0x2f,in_AL % 10 + 0x40);
}



// ---- function FUN_1000_01c8 at 1000:01c8 ----

void __cdecl16near FUN_1000_01c8(void)

{
  code *pcVar1;
  byte extraout_DL;
  undefined2 unaff_DS;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  *(int *)0x9c8 = (uint)(extraout_DL & 3) * 6 + 0xe;
  return;
}



// ---- function FUN_1000_01de at 1000:01de ----

void __cdecl16near FUN_1000_01de(void)

{
  int iVar1;
  char cVar2;
  byte bVar3;
  undefined2 uVar4;
  char cVar5;
  uint uVar6;
  undefined2 unaff_DS;
  
  if ((*(byte *)0x9d0 & 8) != 0) {
    return;
  }
  if (*(char *)0x9d7 != '\0') {
    return;
  }
  uVar4 = 0xff03;
  do {
    cVar5 = (char)((uint)uVar4 >> 8);
    bVar3 = (char)uVar4 + *(char *)0x9b2;
    uVar6 = (uint)bVar3;
    *(uint *)0x9b6 = uVar6;
    if ((*(byte *)(uVar6 + 0xd1) & 0x7f) == 4) {
      while( true ) {
        cVar2 = (char)uVar6 + cVar5;
        *(undefined2 *)0x9b6 = CONCAT11((char)(uVar6 >> 8),cVar2);
        if (cVar2 == *(char *)0x9b2) break;
        uVar6 = *(uint *)0x9b6;
        if (*(char *)(uVar6 + 0xd1) != '\0') goto LAB_1000_026e;
        if (((char)uVar6 != '\\') && ((char)uVar6 != '\x16')) {
          cVar2 = FUN_1000_00f8();
          if (cVar2 != '\0') goto LAB_1000_026e;
          uVar6 = *(uint *)0x9b6;
        }
      }
      *(char *)0x9b4 = cVar2 + cVar5 * -2;
      *(undefined1 *)0x9d1 = 0x40;
      FUN_1000_0000();
      *(char *)0x9b4 = *(char *)0x9b2 - cVar5;
      *(byte *)0x9b2 = bVar3;
      *(undefined1 *)0x9d1 = 0;
      FUN_1000_0000();
      iVar1 = *(int *)0x9ca;
      *(undefined2 *)0x9ca = (undefined2 *)(iVar1 + -6);
      *(undefined2 *)(iVar1 + -6) = 0;
      *(undefined1 *)0x9b2 = *(undefined1 *)0x9b6;
    }
LAB_1000_026e:
    if (cVar5 == '\x01') {
      return;
    }
    uVar4 = 0x1fc;
  } while( true );
}



// ---- function FUN_1000_027a at 1000:027a ----

void __cdecl16near FUN_1000_027a(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return;
}



// ---- function FUN_1000_0298 at 1000:0298 ----

void __cdecl16near FUN_1000_0298(void)

{
  code *pcVar1;
  undefined1 uVar2;
  char cVar3;
  undefined2 uVar4;
  byte in_CH;
  undefined2 unaff_DS;
  
  uVar2 = 0x57;
  if (*(char *)0x9ce != '\0') {
    uVar2 = 0x42;
  }
  *(undefined1 *)0x5e = uVar2;
  *(undefined1 *)0x7d = uVar2;
  *(char *)0x70 = *(char *)0x9d5 + '0';
  if ((*(char *)0x9ce != '\0') || (*(char *)0x9d4 != '\x01')) {
    FUN_1000_027a();
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  FUN_1000_0606();
  FUN_1000_027a();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  *(undefined2 *)0x9c4 = *(undefined2 *)0x9c8;
  if (*(char *)0x17f == '\x01') {
    FUN_1000_05b0();
  }
  FUN_1000_09b4();
  *(undefined1 *)0x9cf = *(undefined1 *)0x9ce;
  FUN_1000_052a();
  if (in_CH == 0) {
    uVar4 = FUN_1000_01ba();
    *(undefined2 *)0x85 = uVar4;
    uVar4 = FUN_1000_01ba();
    *(undefined2 *)0x82 = uVar4;
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  else if ((in_CH & 2) == 0) {
    if ((in_CH & 4) == 0) {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
    }
    else {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
    }
  }
  else {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  if (*(char *)0x17f == -1) {
    FUN_1000_05b0();
  }
  else {
    FUN_1000_0e46();
    cVar3 = FUN_1000_08ec();
    if (cVar3 != '\0') {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
    }
  }
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}



// ---- function FUN_1000_036e at 1000:036e ----

void __cdecl16near FUN_1000_036e(void)

{
  uint uVar1;
  undefined2 unaff_DS;
  
  uVar1 = 0x15;
  do {
    if (*(char *)(uVar1 + 0xd1) != -1) {
      FUN_1000_0386();
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 < 99);
  return;
}



// ---- function FUN_1000_0386 at 1000:0386 ----

undefined4 __cdecl16near FUN_1000_0386(void)

{
  byte *pbVar1;
  uint *puVar2;
  byte bVar3;
  uint in_AX;
  byte bVar4;
  int iVar5;
  char cVar6;
  undefined1 uVar7;
  uint in_DX;
  uint uVar8;
  char cVar9;
  char cVar10;
  uint uVar11;
  byte *pbVar12;
  uint *puVar13;
  uint *puVar14;
  uint uVar15;
  undefined2 unaff_DS;
  
  uVar15 = 0xb800;
  bVar3 = 7 - ((char)((in_AX & 0xff) / 10) + -2);
  bVar4 = (char)((in_AX & 0xff) % 10) - 1;
  uVar11 = 0;
  if ((byte)in_DX == 0x80) {
    uVar11 = 0xffff;
  }
  uVar8 = in_DX;
  if (((bVar4 ^ bVar3) & 1) == 0) {
    uVar11 = ~uVar11;
    uVar8 = (uint)(byte)((byte)in_DX ^ 0x80);
  }
  puVar13 = (uint *)(((uint)bVar3 * 0x18 & 0xff) * 0x28 + (uint)bVar4 * 10);
  bVar3 = (((byte)uVar8 & 7) << 1 | (char)(byte)uVar8 < '\0') - 2;
  if ((char)bVar3 < '\0') {
    cVar9 = '\x18';
    do {
      puVar14 = puVar13;
      for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
        puVar2 = puVar14;
        puVar14 = puVar14 + 1;
        *puVar2 = uVar11;
      }
      puVar13 = puVar14 + -5;
      uVar15 = uVar15 ^ 0x200;
      if ((uVar15 & 0x200) == 0) {
        puVar13 = puVar14 + 0x23;
      }
      cVar9 = cVar9 + -1;
    } while (cVar9 != '\0');
  }
  else {
    pbVar12 = (byte *)((uint)bVar3 * 0x78 + 0xa8c);
    cVar9 = '\x18';
    do {
      cVar6 = '\x05';
      do {
        puVar14 = puVar13;
        pbVar1 = pbVar12;
        pbVar12 = pbVar12 + 1;
        bVar3 = *pbVar1;
        cVar10 = '\b';
        uVar8 = 0;
        do {
          bVar4 = bVar3 << 7;
          bVar3 = bVar3 >> 1 | bVar4;
          uVar7 = (undefined1)(uVar8 >> 2);
          bVar4 = (byte)(uVar8 >> 10) | (char)bVar4 >> 1;
          uVar8 = CONCAT11(bVar4,uVar7);
          cVar10 = cVar10 + -1;
        } while (cVar10 != '\0');
        *puVar14 = CONCAT11(uVar7,bVar4) ^ uVar11;
        cVar6 = cVar6 + -1;
        puVar13 = puVar14 + 1;
      } while (cVar6 != '\0');
      puVar13 = puVar14 + -4;
      uVar15 = uVar15 ^ 0x200;
      if ((uVar15 & 0x200) == 0) {
        puVar13 = puVar14 + 0x24;
      }
      cVar9 = cVar9 + -1;
    } while (cVar9 != '\0');
  }
  return CONCAT22(in_DX,in_AX);
}



// ---- function entry at 1000:0448 ----

/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */
/* WARNING: This function may have set the stack pointer */

void entry(void)

{
  code *pcVar1;
  undefined1 *puVar2;
  
  puVar2 = (undefined1 *)0x200;
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  DAT_10ff_09cf = 0;
  *(undefined2 *)(puVar2 + -2) = 0x466;
  FUN_1000_0964();
  *(undefined2 *)(puVar2 + -2) = 0x469;
  FUN_1000_090e();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  *(undefined2 *)(puVar2 + -2) = 0x471;
  FUN_1000_036e();
  DAT_10ff_09d4 = '\x01';
  do {
    if ((DAT_10ff_09ce & 0x80) == 0) {
      *(undefined2 *)(puVar2 + -2) = 0x480;
      FUN_1000_0298();
      *(undefined2 *)(puVar2 + -2) = 0x483;
      FUN_1000_0bf2();
    }
    else {
      *(undefined2 *)(puVar2 + -2) = 0x489;
      FUN_1000_0bf2();
      *(undefined2 *)(puVar2 + -2) = 0x48c;
      FUN_1000_0298();
    }
    DAT_10ff_09d4 = DAT_10ff_09d4 + '\x01';
  } while( true );
}



// ---- function FUN_1000_0492 at 1000:0492 ----

void __cdecl16near FUN_1000_0492(void)

{
  undefined1 uVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  char cVar5;
  undefined2 unaff_DS;
  
  bVar4 = *(byte *)0x9b2;
  if ((*(byte *)0x9d0 & 0x80) != 0) {
    bVar4 = bVar4 + 10;
  }
  if (((0x3c < bVar4) && (bVar4 < 0x45)) && ((*(byte *)(*(int *)0x9c4 + 4) & 0x10) != 0)) {
    bVar4 = *(byte *)(*(int *)0x9c4 + 3);
    *(byte *)0x9b8 = bVar4;
    bVar2 = *(byte *)(bVar4 + 0xd1);
    *(byte *)0x9d2 = bVar2;
    if ((bVar2 & 7) == 1) {
      cVar5 = bVar4 - *(char *)0x9b4;
      if (cVar5 < '\0') {
        cVar5 = -cVar5;
      }
      if (cVar5 == '\n') {
        *(byte *)0x9d1 = *(byte *)0x9d1 | 0x40;
        FUN_1000_0000();
        *(undefined1 *)0x9b6 = *(undefined1 *)0x9b2;
        uVar1 = *(undefined1 *)0x9b8;
        *(undefined1 *)0x9b2 = uVar1;
        *(undefined1 *)0x9b4 = uVar1;
        *(undefined1 *)0x9d1 = *(undefined1 *)0x9d2;
        FUN_1000_0000();
        *(undefined1 *)0x9b2 = *(undefined1 *)0x9b6;
        iVar3 = *(int *)0x9ca;
        *(undefined2 *)0x9ca = (undefined2 *)(iVar3 + -6);
        *(undefined2 *)(iVar3 + -6) = 0;
      }
    }
  }
  return;
}



// ---- function FUN_1000_050a at 1000:050a ----

void __cdecl16near FUN_1000_050a(void)

{
  char cVar1;
  undefined2 unaff_DS;
  
  FUN_1000_09b4();
  cVar1 = FUN_1000_08ec();
  if (cVar1 == '\0') {
    FUN_1000_0b24();
    FUN_1000_0ca8();
  }
  else {
    *(undefined1 *)0x9d9 = 0;
  }
  FUN_1000_0ec2();
  return;
}



// ---- function FUN_1000_052a at 1000:052a ----

void __cdecl16near FUN_1000_052a(void)

{
  int iVar1;
  undefined2 unaff_DS;
  
  iVar1 = *(int *)0x9c4;
  if (*(char *)0x9cf != *(char *)0x9ce) goto LAB_1000_056c;
  while( true ) {
    if (*(char *)(iVar1 + 3) != *(char *)(iVar1 + 2)) {
      FUN_1000_05e8();
    }
    FUN_1000_05e8();
LAB_1000_056c:
    if ((*(byte *)(iVar1 + 4) & 0x40) == 0) break;
    iVar1 = iVar1 + 6;
  }
  return;
}



// ---- function FUN_1000_05b0 at 1000:05b0 ----

void __cdecl16near FUN_1000_05b0(void)

{
  code *pcVar1;
  char in_CL;
  undefined2 unaff_DS;
  
  if ((char)(*(char *)0x9d3 - *(char *)0x9d4) != '\0') {
    if (in_CL == '\0') {
      *(char *)0xa52 = (*(char *)0x9d3 - *(char *)0x9d4) + '0';
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
    }
    return;
  }
  if (in_CL != '\0') {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    FUN_1000_0894();
    return;
  }
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  FUN_1000_0894();
  return;
}



// ---- function FUN_1000_05e8 at 1000:05e8 ----

void __cdecl16near FUN_1000_05e8(void)

{
  uint uVar1;
  uint in_CX;
  uint uVar2;
  int iVar3;
  char cVar4;
  
  uVar2 = in_CX & 0xff00;
  do {
    FUN_1000_07ae();
    iVar3 = 10000;
    do {
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    FUN_1000_0386();
    cVar4 = (char)uVar2;
    uVar1 = uVar2 >> 8;
    uVar2 = uVar2 ^ 1;
  } while ((cVar4 != '\x01') ||
          (cVar4 = (char)uVar1 + -1, uVar2 = CONCAT11(cVar4,1) ^ 1, cVar4 != '\0'));
  return;
}



// ---- function FUN_1000_0606 at 1000:0606 ----

void __cdecl16near FUN_1000_0606(void)

{
  byte *pbVar1;
  undefined1 *puVar2;
  int iVar3;
  code *pcVar4;
  char cVar5;
  byte bVar6;
  undefined2 in_CX;
  uint uVar7;
  undefined2 extraout_DX;
  byte *pbVar8;
  undefined1 *puVar9;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  pcVar4 = (code *)swi(0x21);
  (*pcVar4)();
  *(undefined2 *)0x9e2 = extraout_DX;
  *(undefined2 *)0x9e4 = in_CX;
  *(undefined1 *)0xa6c = *(undefined1 *)0x9d5;
  if ((*(char *)0x9d4 == '\x01') && (*(undefined1 *)0x17f = 0, (*(byte *)0x9ce & 0x80) == 0)) {
    FUN_1000_01c8();
    return;
  }
  if (*(byte *)0x9d4 < 4) {
    *(undefined1 *)0x9d5 = 1;
  }
  *(undefined1 *)0x9d6 = 0;
  *(undefined2 *)0x9c8 = 0;
  *(undefined2 *)0x9cc = 0x1b2;
  *(undefined2 *)0x9c2 = 0x188;
  *(undefined1 *)0x9cf = *(undefined1 *)0x9ce;
  puVar9 = (undefined1 *)0x17e;
  *(undefined2 *)0x9c6 = 0x17e;
  for (uVar7 = (uint)(byte)(*(char *)0x9d5 + 2); uVar7 != 0; uVar7 = uVar7 - 1) {
    puVar2 = puVar9;
    puVar9 = puVar9 + 1;
    *puVar2 = 0;
  }
  *(undefined1 *)0x9df = 0;
  *(undefined1 *)0x9e0 = 0;
  FUN_1000_0b24();
  FUN_1000_0ca8();
  *(undefined1 *)0x9df = *(undefined1 *)0x9da;
  *(undefined1 *)0x9e0 = *(undefined1 *)0x9de;
  do {
    *(char *)0x9d6 = *(char *)0x9d6 + '\x01';
    *(undefined1 *)0x9d8 = 0;
    FUN_1000_08a2();
    if (*(byte *)0x9d6 < *(byte *)0x9d5) {
      FUN_1000_0e88();
    }
    *(undefined2 *)0x9c4 = *(undefined2 *)0x9c2;
LAB_1000_06bb:
    do {
      while (iVar3 = *(int *)*(undefined2 *)0x9c4, iVar3 == 0) {
        if (*(char *)0x9d8 == '\0') {
          bVar6 = 0x80;
          if (*(char *)0x9d7 != '\0') {
            *(undefined1 *)0x9d3 = *(undefined1 *)0x9d4;
            bVar6 = 0xff;
          }
          goto LAB_1000_0765;
        }
        if (*(char *)0x9d6 == '\x01') goto LAB_1000_07a6;
        FUN_1000_004c();
        pbVar8 = (byte *)*(undefined2 *)0x9c6;
        bVar6 = pbVar8[2];
LAB_1000_076e:
        if (*pbVar8 < bVar6) {
          if (((pbVar8[1] < (byte)-bVar6) && (pbVar8[1] = -bVar6, *(char *)0x9d6 == '\x01')) &&
             (*(undefined2 *)0x9c8 = *(undefined2 *)0x9c4,
             (char)((uint)*(undefined2 *)0x17e >> 8) == -1)) {
            *(char *)0x9d5 = *(char *)0x9d5 + -2;
            if ((*(byte *)0x9ce & 0x80) != 0) {
              *(char *)0x9d3 = *(char *)0x9d3 + -1;
            }
LAB_1000_07a6:
            *(undefined1 *)0x9d5 = *(undefined1 *)0xa6c;
            return;
          }
        }
        else {
          FUN_1000_004c();
        }
      }
      *(int *)0x9c4 = iVar3;
      *(int *)*(undefined2 *)0x9c2 = iVar3;
      if (*(byte *)0x9d5 <= *(byte *)0x9d6) {
        FUN_1000_09b4();
        cVar5 = FUN_1000_08ec();
        if (cVar5 == '\0') {
          if ((*(char *)0x9d6 != *(char *)0x9d5) || (cVar5 = FUN_1000_08ec(), cVar5 == '\0')) {
            FUN_1000_0b24();
            FUN_1000_0ca8();
            FUN_1000_0ec2();
            bVar6 = *(byte *)0x9d9;
LAB_1000_0765:
            *(undefined1 *)0x9d8 = 1;
            pbVar8 = (byte *)*(undefined2 *)0x9c6;
            goto LAB_1000_076e;
          }
          goto LAB_1000_070e;
        }
        FUN_1000_0ec2();
        goto LAB_1000_06bb;
      }
    } while (*(char *)(*(int *)0x9c4 + 5) == '\0');
    FUN_1000_09b4();
LAB_1000_070e:
    pbVar1 = (byte *)0x9cf;
    *pbVar1 = *pbVar1 ^ 0x80;
    if (-1 < (char)*pbVar1) {
      *(char *)0x9d4 = *(char *)0x9d4 + '\x01';
    }
    puVar9 = (undefined1 *)*(undefined2 *)0x9c6;
    puVar9[2] = *puVar9;
    *(undefined2 *)0x9c6 = puVar9 + 1;
    pcVar4 = (code *)swi(0x10);
    (*pcVar4)();
    FUN_1000_0e46();
  } while( true );
}



// ---- function FUN_1000_07ae at 1000:07ae ----

void __cdecl16near FUN_1000_07ae(void)

{
  char *pcVar1;
  code *pcVar2;
  char cVar3;
  int iVar4;
  char *pcVar5;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  undefined1 uVar6;
  
  pcVar2 = (code *)swi(0x21);
  cVar3 = (*pcVar2)();
  if (cVar3 != '\0') {
    pcVar2 = (code *)swi(0x21);
    cVar3 = (*pcVar2)();
    uVar6 = cVar3 == '\0';
    if ((bool)uVar6) {
      pcVar2 = (code *)swi(0x21);
      cVar3 = (*pcVar2)();
      pcVar5 = (char *)0xa6e;
      iVar4 = 10;
      do {
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        pcVar1 = pcVar5;
        pcVar5 = pcVar5 + 1;
        uVar6 = cVar3 == *pcVar1;
      } while (!(bool)uVar6);
      if ((bool)uVar6) {
        (*(code *)*(undefined2 *)((9 - iVar4) * 2 + 0xa78))();
      }
    }
  }
  return;
}



// ---- function FUN_1000_0894 at 1000:0894 ----

void FUN_1000_0894(void)

{
  code *pcVar1;
  undefined2 unaff_DS;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
                    /* WARNING: Could not recover jumptable at 0x0001089d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(ulong)*(uint *)0x9ec)();
  return;
}



// ---- function FUN_1000_08a2 at 1000:08a2 ----

void __cdecl16near FUN_1000_08a2(void)

{
  byte bVar1;
  undefined1 uVar2;
  int iVar3;
  undefined2 unaff_DS;
  
  uVar2 = FUN_1000_08ec();
  *(undefined1 *)0x9d7 = uVar2;
  iVar3 = *(int *)0x9c2;
  *(undefined2 *)(iVar3 + 2) = *(undefined2 *)0x9cc;
  iVar3 = iVar3 + 4;
  *(int *)0x9c2 = iVar3;
  *(int *)0x9ca = iVar3;
  iVar3 = 0x15;
  do {
    *(int *)0x9b2 = iVar3;
    bVar1 = *(byte *)(iVar3 + 0xd1);
    if (((bVar1 != 0) && (bVar1 != 0xff)) &&
       (*(byte *)0x9d0 = bVar1, -1 < (char)(bVar1 ^ *(byte *)0x9cf))) {
      FUN_1000_0a30();
    }
    iVar3 = *(int *)0x9b2 + 1;
  } while ((byte)iVar3 < 99);
  return;
}



// ---- function FUN_1000_08ec at 1000:08ec ----

void __cdecl16near FUN_1000_08ec(void)

{
  byte bVar1;
  char in_AL;
  uint uVar2;
  undefined2 unaff_DS;
  
  uVar2 = (uint)*(byte *)((in_AL < '\0') + 0x179);
  *(uint *)0x9b6 = uVar2;
  bVar1 = *(byte *)(uVar2 + 0xd1);
  *(byte *)0x9d0 = bVar1;
  *(byte *)0x9ba = bVar1 & 7;
  FUN_1000_00f8();
  return;
}



// ---- function FUN_1000_090e at 1000:090e ----

void __cdecl16near FUN_1000_090e(void)

{
  undefined1 *puVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  undefined1 *puVar5;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  puVar5 = (undefined1 *)0xd1;
  for (iVar3 = 0x78; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar5;
    puVar5 = puVar5 + 1;
    *puVar1 = 0xff;
  }
  uVar4 = 0;
  do {
    bVar2 = *(byte *)(uVar4 + 0xc9);
    *(byte *)(uVar4 + 0xe6) = bVar2;
    *(byte *)(uVar4 + 300) = bVar2 | 0x80;
    *(undefined1 *)(uVar4 + 0xf0) = 1;
    *(undefined1 *)(uVar4 + 0x122) = 0x81;
    *(undefined1 *)(uVar4 + 0xfa) = 0;
    *(undefined1 *)(uVar4 + 0x104) = 0;
    *(undefined1 *)(uVar4 + 0x10e) = 0;
    *(undefined1 *)(uVar4 + 0x118) = 0;
    uVar4 = uVar4 + 1;
  } while (uVar4 < 8);
  *(undefined1 *)0x179 = 0x19;
  *(undefined1 *)0x17a = 0x5f;
  *(undefined1 *)0x17b = 0x18;
  *(undefined1 *)0x17c = 0x5e;
  return;
}



// ---- function FUN_1000_0964 at 1000:0964 ----

void __cdecl16near FUN_1000_0964(void)

{
  code *pcVar1;
  char cVar2;
  byte bVar3;
  undefined2 unaff_DS;
  
  do {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    pcVar1 = (code *)swi(0x21);
    cVar2 = (*pcVar1)();
    if (cVar2 == 'W') {
      *(undefined1 *)0x9ce = 0x80;
      goto LAB_1000_0985;
    }
  } while (cVar2 != 'B');
  *(undefined1 *)0x9ce = 0;
LAB_1000_0985:
  do {
    do {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
      pcVar1 = (code *)swi(0x21);
      cVar2 = (*pcVar1)();
      bVar3 = cVar2 - 0x30;
    } while (bVar3 == 0);
  } while (6 < bVar3);
  *(byte *)0x9d5 = bVar3;
  return;
}



// ---- function FUN_1000_09a0 at 1000:09a0 ----

void __cdecl16near FUN_1000_09a0(void)

{
  return;
}



// ---- function FUN_1000_09b4 at 1000:09b4 ----

void __cdecl16near FUN_1000_09b4(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  undefined2 unaff_DS;
  
  iVar6 = *(int *)0x9c4;
  while( true ) {
    bVar1 = *(byte *)(iVar6 + 4);
    bVar2 = *(byte *)(iVar6 + 2);
    bVar3 = *(byte *)(iVar6 + 3);
    bVar4 = *(byte *)(bVar2 + 0xd1);
    uVar5 = (uint)((char)bVar4 < '\0');
    if ((bVar1 & 0x20) == 0) {
      if ((bVar4 & 7) == 5) {
        *(byte *)(uVar5 + 0x17b) = bVar3;
      }
      else if ((bVar4 & 7) == 6) {
        if ((bVar1 & 0x40) != 0) {
          bVar4 = bVar4 | 0x10;
        }
        *(byte *)(uVar5 + 0x179) = bVar3;
      }
    }
    else {
      bVar4 = bVar4 & 0xf8 | 5;
    }
    *(byte *)(bVar3 + 0xd1) = bVar4 | 8;
    *(undefined1 *)(bVar2 + 0xd1) = 0;
    if ((bVar1 & 0x40) == 0) break;
    iVar6 = *(int *)0x9c4 + 6;
  }
  if ((bVar1 & 7) == 5) {
    *(undefined1 *)((uVar5 ^ 1) + 0x17b) = 0;
  }
  return;
}



// ---- function FUN_1000_0a30 at 1000:0a30 ----

void __cdecl16near FUN_1000_0a30(void)

{
  byte bVar1;
  byte bVar2;
  byte extraout_AH;
  undefined2 unaff_DS;
  char in_AF;
  bool bVar3;
  
  bVar1 = *(byte *)0x9d0 & 0x87;
  if (bVar1 == 0x81) {
    bVar1 = 0;
  }
  *(byte *)0x9ba = bVar1 & 7;
  bVar1 = *(byte *)((bVar1 & 0xff07) + 0xbb);
  do {
    *(undefined1 *)0x9b4 = *(undefined1 *)0x9b2;
    while (bVar2 = FUN_1000_0af6(), bVar2 < 2) {
      bVar3 = bVar2 == 0;
      bVar2 = in_AF << 4;
      if (*(byte *)0x9ba < 2) {
        if (bVar1 < 3) {
          in_AF = (bVar2 & 0x10) != 0;
          if (bVar3 || (bVar2 & 0x40) != 0) {
            FUN_1000_0492();
          }
          else {
            if ((0x5a < *(byte *)0x9b4) || (*(byte *)0x9b4 < 0x1d)) {
              *(byte *)0x9d1 = *(byte *)0x9d1 | 0x20;
            }
LAB_1000_0ab6:
            FUN_1000_0000();
          }
          break;
        }
        if (bVar1 == 3) {
          in_AF = (bVar2 & 0x10) != 0;
          if (bVar3 || (bVar2 & 0x40) != 0) goto LAB_1000_0ab6;
          break;
        }
        in_AF = (bVar2 & 0x10) != 0;
        if (!bVar3 && (bVar2 & 0x40) == 0) break;
        if ((0x5a < *(byte *)0x9b4) || (*(byte *)0x9b4 < 0x1d)) {
          *(byte *)0x9d1 = *(byte *)0x9d1 | 0x20;
        }
        FUN_1000_0000();
        bVar1 = bVar1 - 1;
        if ((*(byte *)0x9d0 & 8) != 0) break;
      }
      else {
        FUN_1000_0000();
        in_AF = (extraout_AH & 0x10) != 0;
        if ((((extraout_AH & 0x40) == 0) || (*(byte *)0x9ba == 6)) || (*(byte *)0x9ba < 3)) break;
      }
    }
    bVar1 = bVar1 - 1;
    if (bVar1 == 0) {
      if (*(char *)0x9ba == '\x06') {
        FUN_1000_01de();
      }
      return;
    }
  } while( true );
}



// ---- function FUN_1000_0ad6 at 1000:0ad6 ----

undefined2 __cdecl16near FUN_1000_0ad6(void)

{
  int iVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  char in_CL;
  int *unaff_SI;
  int *piVar4;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  uVar2 = 0;
  if (in_CL != '\0') {
    do {
      piVar4 = unaff_SI;
      unaff_SI = piVar4 + 1;
    } while (*piVar4 == 0);
    iVar1 = *piVar4;
    uVar3 = (undefined1)((uint)iVar1 >> 8);
    *(undefined1 *)piVar4 = uVar3;
    *(undefined1 *)((int)piVar4 + 1) = 0;
    uVar2 = CONCAT11(uVar3,(char)iVar1 * '\x02');
  }
  return uVar2;
}



// ---- function FUN_1000_0af6 at 1000:0af6 ----

undefined1 __cdecl16near FUN_1000_0af6(void)

{
  byte bVar1;
  undefined1 uVar2;
  char in_CL;
  int iVar3;
  undefined2 unaff_DS;
  
  iVar3 = CONCAT11((char)((uint)*(undefined2 *)0x9b4 >> 8),(char)*(undefined2 *)0x9b4 + in_CL);
  *(int *)0x9b4 = iVar3;
  bVar1 = *(byte *)(iVar3 + 0xd1);
  if (bVar1 == 0xff) {
    uVar2 = 3;
  }
  else {
    *(byte *)0x9d1 = bVar1;
    *(byte *)0x9bc = bVar1 & 7;
    uVar2 = 0;
    if ((bVar1 & 7) != 0) {
      if ((char)(*(byte *)0x9d1 ^ *(byte *)0x9d0) < '\0') {
        return 1;
      }
      return 2;
    }
  }
  return uVar2;
}



// ---- function FUN_1000_0b24 at 1000:0b24 ----

void __cdecl16near FUN_1000_0b24(void)

{
  uint *puVar1;
  char cVar2;
  uint uVar3;
  undefined1 uVar4;
  int iVar5;
  byte bVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  *(undefined2 *)0x9c0 = 0;
  for (iVar7 = 0; bVar6 = *(byte *)(iVar7 + 0x179), bVar6 == 0; iVar7 = iVar7 + 1) {
LAB_1000_0bee:
  }
  if (bVar6 == 0xff) {
    return;
  }
  *(uint *)0x9b6 = (uint)bVar6;
  *(undefined1 *)0x9d0 = *(undefined1 *)(bVar6 + 0xd1);
  bVar6 = 8;
  iVar8 = 0;
  do {
    *(undefined1 *)0x9b4 = *(undefined1 *)0x9b6;
    *(undefined1 *)0x9b8 = 0;
    uVar4 = *(undefined1 *)(iVar8 + 0x9c);
LAB_1000_0b60:
    do {
      uVar3 = FUN_1000_0af6();
      cVar2 = (char)uVar3;
    } while (cVar2 == '\0');
    if (cVar2 != '\x03') {
      if (cVar2 == '\x02') {
        if (*(char *)0x9b8 == '\0') break;
      }
      else if (*(char *)0x9b8 != '\0') {
        cVar2 = *(char *)0x9bc;
        if (cVar2 == '\x05') {
          if ((*(byte *)0x9d0 & 7) != 5) goto LAB_1000_0bd2;
          puVar9 = (uint *)0x149;
          for (iVar5 = 0xe; iVar5 != 0; iVar5 = iVar5 + -1) {
            puVar1 = puVar9;
            puVar9 = puVar9 + 1;
            *puVar1 = uVar3 & 0xff00;
          }
          *(undefined1 *)0x9ba = 7;
          FUN_1000_00f8();
          cVar2 = *(char *)0x149 - *(char *)0x157;
          if ((*(byte *)0x9d0 & 0x80) != 0) {
            cVar2 = -cVar2;
          }
          if ((char)(cVar2 + -1) < '\0') {
LAB_1000_0bd2:
            iVar5 = *(int *)0x9c0;
            *(int *)0x9c0 = iVar5 + 1;
            *(undefined1 *)(iVar5 + 0x170) = uVar4;
            *(undefined1 *)(iVar5 + 0x166) = *(undefined1 *)0x9b8;
          }
        }
        else if (bVar6 < 5) {
          if (cVar2 == '\x04') goto LAB_1000_0bd2;
        }
        else if (cVar2 == '\x03') goto LAB_1000_0bd2;
      }
    }
    iVar8 = iVar8 + 1;
    bVar6 = bVar6 - 1;
    if (bVar6 == 0) goto LAB_1000_0bee;
  } while( true );
  *(undefined1 *)0x9b8 = *(undefined1 *)0x9b4;
  goto LAB_1000_0b60;
}



// ---- function FUN_1000_0bf2 at 1000:0bf2 ----

void __cdecl16near FUN_1000_0bf2(void)

{
  byte bVar1;
  code *pcVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  undefined2 unaff_DS;
  bool bVar5;
  
  uVar3 = 0x42;
  uVar4 = *(undefined1 *)0x17a;
  if (*(char *)0x9ce != '\0') {
    uVar3 = 0x57;
    uVar4 = *(undefined1 *)0x179;
  }
  *(undefined1 *)0x10a2 = uVar3;
  *(undefined1 *)0x10c7 = uVar3;
  *(undefined1 *)0x9e6 = uVar4;
  while( true ) {
    do {
      *(undefined1 *)0x9e7 = 1;
      FUN_1000_027a();
      pcVar2 = (code *)swi(0x21);
      (*pcVar2)();
      do {
        FUN_1000_05e8();
      } while (*(char *)0x9e7 == '\x01');
      FUN_1000_027a();
      pcVar2 = (code *)swi(0x21);
      (*pcVar2)();
      do {
        FUN_1000_05e8();
      } while (*(char *)0x9e7 == '\x02');
      bVar1 = *(byte *)0x9e9;
      *(undefined1 *)(bVar1 + 0xd1) = *(undefined1 *)0x9e8;
      bVar5 = bVar1 < *(byte *)0x9ea;
    } while (bVar1 == *(byte *)0x9ea);
    FUN_1000_0f48();
    if (!bVar5) break;
    FUN_1000_027a();
    pcVar2 = (code *)swi(0x21);
    (*pcVar2)();
    *(undefined1 *)0x9e6 = *(undefined1 *)0x9e9;
    FUN_1000_0386();
  }
  FUN_1000_0386();
  FUN_1000_052a();
  return;
}



// ---- function FUN_1000_0ca8 at 1000:0ca8 ----

void __cdecl16near FUN_1000_0ca8(void)

{
  uint *puVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  uint in_AX;
  uint uVar5;
  undefined1 uVar6;
  undefined1 extraout_AH;
  undefined1 extraout_AH_00;
  int iVar7;
  byte extraout_DL;
  char extraout_DL_00;
  char extraout_DH;
  uint uVar8;
  uint *puVar9;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  uVar5 = in_AX & 0xff00;
  *(undefined1 *)0x9de = 0;
  *(undefined1 *)0x9da = 0;
  *(undefined1 *)0x9db = 0;
  *(undefined1 *)0x9dc = 0;
  *(undefined1 *)0x9dd = 0;
  *(undefined1 *)0x9e1 = 0;
  *(undefined1 *)0x9ba = 7;
  uVar8 = 0x15;
  do {
    *(uint *)0x9b6 = uVar8;
    bVar3 = *(byte *)(uVar8 + 0xd1);
    uVar6 = (undefined1)(uVar5 >> 8);
    uVar5 = CONCAT11(uVar6,bVar3);
    if (bVar3 != 0xff) {
      *(byte *)0x9d0 = bVar3;
      uVar5 = CONCAT11(uVar6,bVar3) & 0xff07;
      bVar2 = (byte)uVar5;
      *(byte *)0x9be = bVar2;
      if (1 < bVar2) {
        uVar6 = (undefined1)(uVar5 >> 8);
        if (bVar2 < 4) {
          if ((bVar3 & 8) == 0) {
LAB_1000_0d0f:
            uVar5 = CONCAT11(uVar6,2);
            if ((bVar3 & 0x80) == 0) {
              uVar5 = CONCAT11(uVar6,0xfe);
            }
            goto LAB_1000_0d18;
          }
        }
        else if ((bVar2 == 6) && ((bVar3 & 0x10) != 0)) {
          uVar5 = CONCAT11(uVar6,6);
          if ((bVar3 & 0x80) != 0) {
            uVar5 = CONCAT11(uVar6,0xfa);
          }
LAB_1000_0d18:
          *(char *)0x9da = *(char *)0x9da + (char)uVar5;
        }
        else if ((bVar3 & 8) != 0) goto LAB_1000_0d0f;
      }
      puVar9 = (uint *)0x149;
      for (iVar7 = 0xe; iVar7 != 0; iVar7 = iVar7 + -1) {
        puVar1 = puVar9;
        puVar9 = puVar9 + 1;
        *puVar1 = uVar5 & 0xff00;
      }
      FUN_1000_00f8();
      uVar5 = CONCAT11(extraout_AH,*(char *)0x149 - *(char *)0x157);
      *(char *)0x9da = *(char *)0x9da + (*(char *)0x149 - *(char *)0x157);
      if (*(char *)0x9d0 != '\0') {
        FUN_1000_0f9e();
        cVar4 = extraout_DH;
        if (extraout_DL != 0) {
          cVar4 = extraout_DH + -1;
          if ((char)(*(byte *)0x9d0 ^ *(byte *)0x9cf) < '\0') {
            bVar3 = extraout_DL;
            if (*(byte *)0x9dc <= extraout_DL) {
              LOCK();
              bVar3 = *(byte *)0x9dc;
              *(byte *)0x9dc = extraout_DL;
              UNLOCK();
            }
            if (*(byte *)0x9dd <= bVar3) {
              *(byte *)0x9dd = bVar3;
            }
          }
          else if (*(byte *)0x9db <= extraout_DL) {
            *(byte *)0x9db = extraout_DL;
            if (*(char *)0x9b6 == *(char *)(*(int *)0x9c4 + 3)) {
              *(char *)0x9e1 = *(char *)0x9b6;
            }
          }
        }
        if ((*(byte *)0x9d0 & 0x80) != 0) {
          cVar4 = -cVar4;
        }
        uVar5 = CONCAT11(extraout_AH_00,cVar4);
        *(char *)0x9de = *(char *)0x9de + cVar4;
      }
    }
    uVar8 = *(int *)0x9b6 + 1;
    if (0x62 < uVar8) {
      if (*(char *)0x9e1 != '\0') {
        *(undefined1 *)0x9dc = *(undefined1 *)0x9dd;
        *(undefined1 *)0x9dd = 0;
      }
      FUN_1000_09a0();
      cVar4 = FUN_1000_09a0();
      cVar4 = cVar4 + extraout_DL_00 * '\x04';
      if ((*(byte *)0x9cf & 0x80) == 0) {
        cVar4 = -cVar4;
      }
      *(char *)0x9d9 = cVar4 + -0x80;
      *(char *)(*(int *)0x9c4 + 5) = cVar4 + -0x80;
      return;
    }
  } while( true );
}



// ---- function FUN_1000_0e18 at 1000:0e18 ----

void __cdecl16near FUN_1000_0e18(void)

{
  code *pcVar1;
  byte in_AL;
  byte bVar2;
  
  bVar2 = 100;
  if (in_AL < 100) {
    bVar2 = 10;
  }
  do {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    bVar2 = bVar2 / 10;
  } while (bVar2 != 0);
  return;
}



// ---- function FUN_1000_0e46 at 1000:0e46 ----

void __cdecl16near FUN_1000_0e46(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  FUN_1000_0e18();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  FUN_1000_0e18();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  FUN_1000_0e18();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}



// ---- function FUN_1000_0e88 at 1000:0e88 ----

void __cdecl16near FUN_1000_0e88(void)

{
  int iVar1;
  int *piVar2;
  undefined2 *puVar3;
  undefined2 *puVar4;
  undefined2 unaff_DS;
  
  piVar2 = (int *)*(undefined2 *)0x9c2;
  puVar4 = (undefined2 *)0x0;
  do {
    iVar1 = *piVar2;
    *piVar2 = (int)puVar4;
    if (iVar1 == 0) {
      return;
    }
    *(int *)0x9c4 = iVar1;
    FUN_1000_050a();
    piVar2 = (int *)*(undefined2 *)0x9c4;
    puVar4 = (undefined2 *)*(undefined2 *)0x9c2;
    do {
      puVar3 = puVar4;
      puVar4 = (undefined2 *)*puVar3;
      if (puVar4 == (undefined2 *)0x0) break;
    } while (*(byte *)((int)puVar4 + 5) <= *(byte *)0x9d9);
    *puVar3 = piVar2;
  } while( true );
}



// ---- function FUN_1000_0ec2 at 1000:0ec2 ----

void __cdecl16near FUN_1000_0ec2(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  undefined2 unaff_DS;
  
  iVar6 = *(int *)0x9c4;
  while( true ) {
    bVar1 = *(byte *)(iVar6 + 4);
    bVar2 = *(byte *)(iVar6 + 2);
    bVar3 = *(byte *)(iVar6 + 3);
    bVar4 = *(byte *)(bVar3 + 0xd1);
    uVar5 = (uint)((char)bVar4 < '\0');
    if ((bVar1 & 0x20) == 0) {
      if ((bVar4 & 7) == 5) {
        *(byte *)(uVar5 + 0x17b) = bVar2;
      }
      else if ((bVar4 & 7) == 6) {
        if ((bVar1 & 0x40) != 0) {
          bVar4 = bVar4 & 0xef;
        }
        *(byte *)(uVar5 + 0x179) = bVar2;
      }
    }
    else {
      bVar4 = bVar4 & 0xf8 | 1;
    }
    if ((bVar1 & 0x10) != 0) {
      bVar4 = bVar4 & 0xf7;
    }
    *(byte *)(bVar2 + 0xd1) = bVar4;
    *(byte *)(bVar3 + 0xd1) = bVar1 & 0x8f;
    if ((bVar1 & 0x40) == 0) break;
    iVar6 = *(int *)0x9c4 + 6;
  }
  if ((bVar1 & 7) == 5) {
    *(byte *)((uVar5 ^ 1) + 0x17b) = bVar3;
  }
  return;
}



// ---- function FUN_1000_0f48 at 1000:0f48 ----

undefined2 __cdecl16near FUN_1000_0f48(void)

{
  undefined2 uVar1;
  char cVar2;
  undefined2 uVar3;
  undefined2 in_DX;
  int *piVar4;
  undefined2 unaff_DS;
  
  uVar1 = *(undefined2 *)0x9c4;
  *(byte *)0x9cf = *(byte *)0x9ce ^ 0x80;
  *(undefined2 *)0x9c2 = 0x188;
  *(undefined2 *)0x9cc = 0x5b2;
  uVar3 = FUN_1000_08a2();
  piVar4 = (int *)0x5b2;
  while (((char)in_DX != (char)piVar4[1] || ((char)((uint)in_DX >> 8) != *(char *)((int)piVar4 + 3))
         )) {
    piVar4 = (int *)*piVar4;
    if (piVar4 == (int *)0x0) {
LAB_1000_0f98:
      *(undefined2 *)0x9c4 = uVar1;
      return uVar3;
    }
  }
  *(undefined2 *)0x9c4 = piVar4;
  FUN_1000_09b4();
  cVar2 = FUN_1000_08ec();
  if (cVar2 == '\0') {
    return uVar1;
  }
  uVar3 = FUN_1000_0ec2();
  goto LAB_1000_0f98;
}



// ---- function FUN_1000_0f9e at 1000:0f9e ----

/* WARNING: Removing unreachable block (ram,0x00010ff0) */

void __cdecl16near FUN_1000_0f9e(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined2 unaff_DS;
  undefined1 uVar4;
  bool bVar5;
  
  bVar3 = *(char *)(*(int *)0x9be + 0xc2) << 1;
  uVar4 = bVar3 == 0;
  bVar1 = FUN_1000_0ad6();
  if (!(bool)uVar4) {
    while( true ) {
      bVar2 = bVar1;
      bVar1 = FUN_1000_0ad6();
      if ((!(bool)uVar4) && (bVar3 < bVar2)) {
        do {
          bVar5 = bVar1 == bVar2;
          if (bVar1 < bVar2) {
            return;
          }
          bVar2 = FUN_1000_0ad6();
          if (bVar5) {
            return;
          }
          bVar1 = FUN_1000_0ad6();
        } while (!bVar5);
      }
      bVar3 = bVar2;
      if (bVar1 == 0) break;
      uVar4 = 0;
    }
  }
  return;
}



