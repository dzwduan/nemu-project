#include "cc.h"

static inline def_EHelper(test)
{
  *s0 = *ddest&*dsrc1;
  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  //operand_write(s, id_dest, s0);
  print_asm_template2(test);
}

static inline def_EHelper(and)
{
  //printf("before and esp is %d\n",cpu.esp);
  cpu.eflags.CF = cpu.eflags.OF = 0;
  *s0 = *ddest & *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  //printf("after and esp is %d\n",cpu.esp);
  print_asm_template2(and);
}

static inline def_EHelper(xor)
{
  cpu.eflags.CF = cpu.eflags.OF = 0;
  *s0 = *ddest ^ *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template2(xor);
}

static inline def_EHelper(or)
{
  cpu.eflags.CF = cpu.eflags.OF = 0;
  *s0 = *ddest | *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template2(or);
}

static inline def_EHelper(not )
{
  cpu.eflags.CF = cpu.eflags.OF = 0;
  *s0 = ~(*ddest); 
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template1(not );
}

//算术右
static inline def_EHelper(sar)
{
  *s0 = (*dsrc1) & c_shift_mask; //移动位数
  // *s1 = *ddest;                  //要移动的数
  // rtl_msb(s, s2, ddest, id_dest->width); //s2是符号位
  *s1 = *ddest>>(*s0);
  *s2 = ((*s1)^(1<<(8*id_dest->width-1-(*s0)))) - (1<<(8*id_dest->width-1-(*s0)));

  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(s,s2,id_dest->width);
  operand_write(s, id_dest, s2);
  // unnecessary to update eflags.CF and eflags.OF in NEMU
  print_asm_template2(sar);
}

static inline def_EHelper(shl)
{
  *s0 = *ddest<<*dsrc1;
  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  operand_write(s, id_dest, s0);
  // unnecessary to update eflags.CF and eflags.OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr)
{
  *s0 = *ddest>>*dsrc1;
  cpu.eflags.CF = cpu.eflags.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  operand_write(s, id_dest, s0);
  // unnecessary to update eflags.CF and eflags.OF in NEMU
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
