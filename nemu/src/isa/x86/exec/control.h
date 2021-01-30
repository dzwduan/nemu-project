#include "cc.h"


static inline def_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(s, s->jmp_pc);

  print_asm("jmp %x", s->jmp_pc);
}

static inline def_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, s0, cc);
  rtl_jrelop(s, RELOP_NE, s0, rz, s->jmp_pc);

  print_asm("j%s %x", get_cc_name(cc), s->jmp_pc);
}

static inline def_EHelper(jmp_rm) {
  rtl_jr(s, ddest);

  print_asm("jmp *%s", id_dest->str);
}

static inline def_EHelper(call) {
  // the target address is calculated at the decode stage
  rtl_push(s,&s->seq_pc);
  s->is_jmp=0;
  s->seq_pc=s->jmp_pc;
  print_asm("call %x", s->jmp_pc);
}

static inline def_EHelper(ret) {
  rtl_pop(s,&s->jmp_pc);
  s->is_jmp=0;
  s->seq_pc=s->jmp_pc;
  print_asm("ret");
}

static inline def_EHelper(ret_imm) {
  TODO();
  print_asm("ret %s", id_dest->str);
}

/*
IF r/m16 or r/m32 type of call
THEN (* near absolute call *)
   IF OperandSize = 16
   THEN
      Push(IP);
      EIP := [r/m16] AND 0000FFFFH;
   ELSE (* OperandSize = 32 *)
      Push(EIP);
      EIP := [r/m32];
   FI;
FI;
*/
static inline def_EHelper(call_rm) {
  if(s->isa.is_operand_size_16){
    rtl_push(s,&s->seq_pc);
    rtl_andi(s,&s->seq_pc,ddest,0xFFFF);
  }
  else{
    rtl_push(s,&s->seq_pc);
    rtl_jr(s,ddest); 
  }
  print_asm("call *%s", id_dest->str);
}

//根据cx的值，重复执行后面的指令
static inline def_EHelper(rep){
  if(cpu.eax==0) return;
  if(s->isa.is_operand_size_16){
    *s0 = cpu.cx;
    while((*s0)--){
      *s2 = cpu.pc;
      *t0=isa_exec_once();
      cpu.pc= *s2;
    }
  }
    else{
      *s0 = cpu.eax;
      while((*s0)--){
        *s2 = cpu.pc;
        *t0=isa_exec_once();
        cpu.pc= *s2;
      }
    }
    print_asm("rep ");
}
