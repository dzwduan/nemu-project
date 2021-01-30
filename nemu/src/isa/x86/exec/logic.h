#include "cc.h"
/*
两个操作数进行按位“与”运算，与 AND 类似，
但是 AND 要将结果保存到目的操作数中，而TEST 不用保存结果，只影响标志位。
*/
static inline def_EHelper(test)
{
  rtl_and(s, s0, ddest, dsrc1);

  rtl_update_ZFSF(s, s0, id_dest->width);
  *s1 = 0;
  rtl_set_OF(s, s1);
  rtl_set_CF(s, s1);
  print_asm_template2(test);
}

static inline def_EHelper(and)
{
  rtl_and(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s1 = 0;
  rtl_set_OF(s, s1);
  rtl_set_CF(s, s1);
  print_asm_template2(and);
}

static inline def_EHelper(xor)
{
  rtl_xor(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s1 = 0;
  rtl_set_OF(s, s1);
  rtl_set_CF(s, s1);

  print_asm_template2(xor);
}

static inline def_EHelper(or)
{
  rtl_or(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s1 = 0;
  rtl_set_OF(s, s1);
  rtl_set_CF(s, s1);
  print_asm_template2(or);
}

static inline def_EHelper(not )
{
  rtl_not(s, s0, ddest);
  operand_write(s, id_dest, s0);
  print_asm_template1(not);
  print_asm_template1(not);
}

//算术右移
static inline def_EHelper(sar)
{
  rtl_sar(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(sar);
}

//逻辑右移
static inline def_EHelper(shl)
{
  rtl_shl(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr)
{
  rtl_shr(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}

static inline def_EHelper(setcc)
{
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

//从高位向低位找到第一个1
static inline def_EHelper(bsr)
{
  cpu.eflags.ZF = (*dsrc1 == 0);
  *s0 = id_dest->width*8-1;
  *s1 = 0;
  *t0 = 0;
  for(;(*s0)>=0;(*s0)--){
    if((*dsrc1 >> *s0 &1)==1) break;
    else (*t0)++;
  }
  operand_write(s,id_dest,t0);
  print_asm_template2(bsr);
}

//循环左移
static inline def_EHelper(rol){
  rtl_mv(s,s0,dsrc1);
  if(s->isa.is_operand_size_16){
    *ddest = (*ddest >>(16-*s0) | (*ddest << *s0));
  }
  else
    *ddest = (*ddest >>(32-*s0) | (*ddest << *s0));
  print_asm_template2(rol);
}

//循环右移
static inline def_EHelper(ror){
  rtl_mv(s,s0,dsrc1);  //移动的位数
  if(s->isa.is_operand_size_16){
    *ddest = (*ddest <<(16-*s0) | (*ddest >> *s0));
  }
  else
    *ddest = (*ddest <<(32-*s0) | (*ddest >> *s0));
  print_asm_template2(ror);
}
