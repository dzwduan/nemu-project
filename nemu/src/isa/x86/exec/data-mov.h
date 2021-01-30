static inline def_EHelper(mov)
{
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push)
{
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop)
{
  rtl_pop(s, s0);
  operand_write(s, id_dest, s0); //注意不能缺少
  print_asm_template1(pop);
}

/*
IF OperandSize = 16 (* PUSHA instruction *)
THEN
   Temp := (SP);
   Push(AX);
   Push(CX);
   Push(DX);
   Push(BX);
   Push(Temp);
   Push(BP);
   Push(SI);
   Push(DI);
ELSE (* OperandSize = 32, PUSHAD instruction *)
   Temp := (ESP);
   Push(EAX);
   Push(ECX);
   Push(EDX);
   Push(EBX);
   Push(Temp);
   Push(EBP);
   Push(ESI);
   Push(EDI);
FI;
*/
static inline def_EHelper(pusha)
{
  if (s->isa.is_operand_size_16)
  {
    *s1 = cpu.sp; //注意要第一个获取当前sp
    *s0 = cpu.ax;
    rtl_push(s, s0);
    *s0 = cpu.cx;
    rtl_push(s, s0);
    *s0 = cpu.dx;
    rtl_push(s, s0);
    *s0 = cpu.bx;
    rtl_push(s, s0);
    rtl_push(s, s1);
    *s0 = cpu.bp;
    rtl_push(s, s0);
    *s0 = cpu.si;
    rtl_push(s, s0);
    *s0 = cpu.di;
    rtl_push(s, s0);
  }
  else
  {
    rtl_mv(s, s0, &cpu.esp);
    rtl_push(s, &cpu.eax);
    rtl_push(s, &cpu.ecx);
    rtl_push(s, &cpu.edx);
    rtl_push(s, &cpu.ebx);
    rtl_push(s, s0);
    rtl_push(s, &cpu.ebp);
    rtl_push(s, &cpu.esi);
    rtl_push(s, &cpu.edi);
  }
  print_asm("pusha");
}

/*
IF OperandSize = 16 (* instruction = POPA *)
THEN
   DI := Pop();
   SI := Pop();
   BP := Pop();
   throwaway := Pop (); (* Skip SP *)
   BX := Pop();
   DX := Pop();
   CX := Pop();
   AX := Pop();
ELSE (* OperandSize = 32, instruction = POPAD *)
   EDI := Pop();
   ESI := Pop();
   EBP := Pop();
   throwaway := Pop (); (* Skip ESP *)
   EBX := Pop();
   EDX := Pop();
   ECX := Pop();
   EAX := Pop();
FI
*/
static inline def_EHelper(popa)
{
  if (s->isa.is_operand_size_16)
  {
    //注意类型

    // rtl_pop(s,&cpu.di);
    // rtl_pop(s,&cpu.si);
    // rtl_pop(s,&cpu.bp);
    // rtl_pop(s,s0);
    // rtl_pop(s,&cpu.bx);
    // rtl_pop(s,&cpu.dx);
    // rtl_pop(s,&cpu.cx);
    // rtl_pop(s,&cpu.ax);
  }
  else
  {
    rtl_pop(s, &cpu.edi);
    rtl_pop(s, &cpu.esi);
    rtl_pop(s, &cpu.ebp);
    rtl_pop(s, s0);
    rtl_pop(s, &cpu.ebx);
    rtl_pop(s, &cpu.edx);
    rtl_pop(s, &cpu.ecx);
    rtl_pop(s, &cpu.eax);
  }
  print_asm("popa");
}

/*
Set SP to BP, then pop BP
*/
static inline def_EHelper(leave)
{
  if (s->isa.is_operand_size_16)
  {
    cpu.sp = *(int16_t *)&cpu.bp;
    rtl_pop(s, s0);
    cpu.bp = *s0;
  }
  else
  {
    cpu.esp = *(int32_t *)&cpu.ebp;
    rtl_pop(s, &cpu.ebp);
  }
  print_asm("leave");
}

/*
这个指令就是简单的将%eax寄存器的值符号扩展32位到%edx寄存器，
如果%eax寄存器的二进制序列的最高位为0，则cltd指令将把%edx置为32个0，
相反，如果%eax寄存器的二进制序列最高位为1，则cltd指令将会自从填充%edx寄存器为32个1。
*/
static inline def_EHelper(cltd)
{
  if (s->isa.is_operand_size_16)
  {
    rtl_msb(s, s0, &cpu.eax, 2);
    if (*s0 == 1)
      cpu.dx |= 0xFFFF;
    else
      cpu.dx &= 0x0000;
  }
  else
  {
    rtl_msb(s, s0, &cpu.eax, 4);
    if (*s0 == 1)
      cpu.edx |= 0xFFFFFFFF;
    else
      cpu.edx &= 0x00000000;
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

/*
sign-extend word in %ax to long in %eax
*/
static inline def_EHelper(cwtl)
{
  if (s->isa.is_operand_size_16)
  {
    // extend
    cpu.ax = *(int8_t *)&cpu.al;
  }
  else
  {
    cpu.eax = *(int16_t *)&cpu.ax;
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx)
{
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

//[(E)SI] to ES:[(E)DI]
//这里寄存器也是内存模拟，用完后要加上地址移动
static inline def_EHelper(movsb)
{
  rtl_lr(s,s0,R_ESI,id_dest->width);
  rtl_sr(s,R_EDI,s0,id_dest->width);
  *s1 = id_dest->width;
  rtl_add(s,&cpu.esi,&cpu.esi,s1);
  rtl_add(s,&cpu.edi,&cpu.edi,s1);
  print_asm_template2(mov);

}

static inline def_EHelper(movzx)
{
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_zext(s,ddest,dsrc1,id_dest->width);
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea)
{
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}

static inline def_EHelper(xchg)
{
  *s0 = *ddest;
  operand_write(s, id_dest, dsrc1);
  operand_write(s, id_src1, s0);
}

static inline def_EHelper(r2cr) {
  *dsrc1 = *ddest;
}

static inline def_EHelper(cr2r) {
  *ddest = *dsrc1;
}