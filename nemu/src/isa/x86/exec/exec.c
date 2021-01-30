#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width)
{
  if (width == -1)
    return;
  if (width == 0)
  {
    width = s->isa.is_operand_size_16 ? 2 : 4;
  }
  s->src1.width = s->dest.width = s->src2.width = width;
}

/* 0x80, 0x81, 0x83 */
static inline def_EHelper(gp1)
{
  switch (s->isa.ext_opcode)
  {
    EX(0, add)
    EMPTY(1)
    EMPTY(2)
    EMPTY(3)
    EX(4, and)
    EX(5, sub)
    EMPTY(6) EX(7, cmp)
  }
}

/* 0xc0, 0xc1, 0xd0, 0xd1, 0xd2, 0xd3 */
static inline def_EHelper(gp2)
{
  switch (s->isa.ext_opcode)
  { 
    //循环左移
    EX(0,rol)
    //循环右移
    EX(1,ror)
    EMPTY(2)
    EMPTY(3)
    EX(4,shl)
    EMPTY(5)
    EMPTY(6)
    EX(7,sar)
  }
}

/* 0xf6, 0xf7 */
static inline def_EHelper(gp3)
{
  switch (s->isa.ext_opcode)
  {
    EMPTY(0)
    EMPTY(1)
    EX(2,not)
    EX(3,neg)
    EX(4,mul)
    EX(5,imul1)
    EX(6,div) 
    EX(7,idiv)
  }
}

/* 0xfe */
static inline def_EHelper(gp4)
{
  switch (s->isa.ext_opcode)
  {
    EX(0,inc)
    EMPTY(1)
    EMPTY(2)
    EMPTY(3)
    EMPTY(4)
    EMPTY(5)
    EMPTY(6) EMPTY(7)
  }
}

/* 0xff */
static inline def_EHelper(gp5)
{
  switch (s->isa.ext_opcode)
  {
    EX(0, inc)
    EX(1,dec)
    EX(2,call_rm)
    EMPTY(3)
    EX(4, jmp_rm)
    EMPTY(5)
    EX(6, push) 
    EMPTY(7)
  }
}

/* 0x0f 0x01*/
static inline def_EHelper(gp7)
{
  switch (s->isa.ext_opcode)
  {
    EMPTY(0)
    EMPTY(1)
    EMPTY(2)
    EMPTY(3)
    EMPTY(4)
    EMPTY(5)
    EMPTY(6) EMPTY(7)
  }
}

static inline def_EHelper(2byte_esc)
{
  uint8_t opcode = instr_fetch(&s->seq_pc, 1);
  s->opcode = opcode;
  printf("Now exec instruction 0x0f %x\n", s->opcode);
  switch (opcode)
  {
    /* TODO: Add more instructions!!! */
    IDEX(0x01, gp7_E, gp7)
    //he reg field within the ModRM byte specifies
    // which of the special registers in each category is involved
    // IDEX(0x20, E2G, cr2r)
    // IDEX(0x22, G2E, r2cr)

    IDEX(0x80, J, jcc)
    IDEX(0x81, J, jcc)
    IDEX(0x82, J, jcc)
    IDEX(0x83, J, jcc)
    IDEX(0x84, J, jcc)
    IDEX(0x85, J, jcc)
    IDEX(0x86, J, jcc)
    IDEX(0x87, J, jcc)
    IDEX(0x88, J, jcc)
    IDEX(0x89, J, jcc)
    IDEX(0x8a, J, jcc)
    IDEX(0x8b, J, jcc)
    IDEX(0x8c, J, jcc)
    IDEX(0x8d, J, jcc)
    IDEX(0x8e, J, jcc)
    IDEX(0x8f, J, jcc)
    IDEXW(0x90, setcc_E, setcc, 1)
    IDEXW(0x91, setcc_E, setcc, 1)
    IDEXW(0x92, setcc_E, setcc, 1)
    IDEXW(0x93, setcc_E, setcc, 1)
    IDEXW(0x94, setcc_E, setcc, 1)
    IDEXW(0x95, setcc_E, setcc, 1)
    IDEXW(0x96, setcc_E, setcc, 1)
    IDEXW(0x97, setcc_E, setcc, 1)
    IDEXW(0x98, setcc_E, setcc, 1)
    IDEXW(0x99, setcc_E, setcc, 1)
    IDEXW(0x9a, setcc_E, setcc, 1)
    IDEXW(0x9b, setcc_E, setcc, 1)
    IDEXW(0x9c, setcc_E, setcc, 1)
    IDEXW(0x9d, setcc_E, setcc, 1)
    IDEXW(0x9e, setcc_E, setcc, 1)
    IDEXW(0x9f, setcc_E, setcc, 1)
    IDEX(0xa4, Ib_G2E, shl)
    IDEX(0xa5, cl_G2E, shl)
    IDEX(0xaf, E2G, imul2)
    IDEXW(0xb6, mov_E2G, movzx, 1)
    IDEXW(0xb7, mov_E2G, movzx, 2)
    IDEXW(0xbe, mov_E2G, movsx, 1)
    IDEX(0xbd, E2G, bsr)
    IDEXW(0xbf, mov_E2G, movsx, 2)
  default:
    exec_inv(s);
  }
}

static inline void fetch_decode_exec(DecodeExecState *s)
{
  uint8_t opcode;
again:
  opcode = instr_fetch(&s->seq_pc, 1); //opcode取第一个字节
  s->opcode = opcode;
  printf("Now exec instruction 0x%x\n", s->opcode);
  switch (opcode)
  {
    //0x00-0x05 add
    IDEX(0x01, G2E, add)
    IDEXW(0x02, E2G, add, 1)
    IDEX(0x03, E2G, add)
    IDEXW(0x04, I2a, add, 1)
    IDEX(0x05, I2a, add)
    //0x08-0x0d or
    IDEXW(0x08, G2E, or, 1)
    IDEX(0x09, G2E, or)
    IDEXW(0x0a, E2G, or, 1)
    IDEX(0x0b, E2G, or)
    IDEXW(0x0c, I2a, or, 1)
    IDEX(0x0d, I2a, or)
    //special 2-byte escape
    EX(0x0f, 2byte_esc)
    //0x10-0x15 adc
    IDEXW(0x10, G2E, adc, 1)
    IDEX(0x11, G2E, adc)
    IDEXW(0x12, G2E, adc, 1)
    IDEX(0x13, G2E, adc)
    IDEXW(0x14, I2a, adc, 1)
    IDEX(0x15, I2a, adc)
    //0x18-0x1d sbb
    IDEXW(0x18, G2E, sbb, 1)
    IDEX(0x19, G2E, sbb)
    IDEXW(0x1a, E2G, sbb, 1)
    IDEX(0x1b, E2G, sbb)
    IDEXW(0x1c, I2a, sbb, 1)
    IDEX(0x1d, I2a, sbb)
    //0x20 - 0x25 and
    IDEXW(0x20, G2E, and, 1)
    IDEX(0x21, G2E, adc)
    IDEXW(0x22, G2E, and, 1)
    IDEX(0x23, G2E, and)
    IDEXW(0x24, I2a, and, 1)
    IDEX(0x25, I2a, and)
    //0x28 - 0x2d sub
    IDEXW(0x28, G2E, sub, 1)
    IDEX(0x29, G2E, sub)
    IDEXW(0x2a, E2G, sub, 1)
    IDEX(0x2b, E2G, sub)
    IDEXW(0x2c, I2a, sub, 1)
    IDEX(0x2d, I2a, sub)
    //0x30 - 0x35 xor
    IDEXW(0x30, G2E, xor, 1)
    IDEX(0x31, G2E, xor)
    IDEXW(0x32, G2E, xor, 1)
    IDEX(0x33, G2E, xor)
    IDEXW(0x34, I2a, xor, 1)
    IDEX(0x35, I2a, xor)
    //0x38 - 0x3d cmp
    IDEXW(0x38, G2E, cmp, 1)
    IDEX(0x39, G2E, cmp)
    IDEXW(0x3a, E2G, cmp, 1)
    IDEX(0x3b, E2G, cmp)
    IDEXW(0x3c, I2a, cmp, 1)
    IDEX(0x3d, I2a, cmp)

    //0x40-0x47 inc
    IDEX(0x40, r, inc)
    IDEX(0x41, r, inc)
    IDEX(0x42, r, inc)
    IDEX(0x43, r, inc)
    IDEX(0x44, r, inc)
    IDEX(0x45, r, inc)
    IDEX(0x46, r, inc)
    IDEX(0x47, r, inc)
    //0x48-0x4f dec
    IDEX(0x48, r, dec)
    IDEX(0x49, r, dec)
    IDEX(0x4a, r, dec)
    IDEX(0x4b, r, dec)
    IDEX(0x4c, r, dec)
    IDEX(0x4d, r, dec)
    IDEX(0x4e, r, dec)
    IDEX(0x4f, r, dec)
    //0x50-0x57 push
    IDEX(0x50, r, push)
    IDEX(0x51, r, push)
    IDEX(0x52, r, push)
    IDEX(0x53, r, push)
    IDEX(0x54, r, push)
    IDEX(0x55, r, push)
    IDEX(0x56, r, push)
    IDEX(0x57, r, push)
    //0x5800x5f pop
    IDEX(0x58, r, pop)
    IDEX(0x59, r, pop)
    IDEX(0x5a, r, pop)
    IDEX(0x5b, r, pop)
    IDEX(0x5c, r, pop)
    IDEX(0x5d, r, pop)
    IDEX(0x5e, r, pop)
    IDEX(0x5f, r, pop)
    //0x60 pusha
    EX(0x60, pusha)
    //0x61 popa
    EX(0x61, popa)
    IDEX(0x68, push_SI, push)
    IDEX(0x69, I_E2G, imul3) //imul with 3 operations
    IDEXW(0x6a, push_SI, push, 1)
    IDEXW(0x6b, I_E2G, imul3, 1)

    //0x70-0x7f short jmp
    IDEXW(0x70, J, jcc, 1)
    IDEXW(0x71, J, jcc, 1)
    IDEXW(0x72, J, jcc, 1)
    IDEXW(0x73, J, jcc, 1)
    IDEXW(0x74, J, jcc, 1)
    IDEXW(0x75, J, jcc, 1)
    IDEXW(0x76, J, jcc, 1)
    IDEXW(0x77, J, jcc, 1)
    IDEXW(0x78, J, jcc, 1)
    IDEXW(0x79, J, jcc, 1)
    IDEXW(0x7a, J, jcc, 1)
    IDEXW(0x7b, J, jcc, 1)
    IDEXW(0x7c, J, jcc, 1)
    IDEXW(0x7d, J, jcc, 1)
    IDEXW(0x7e, J, jcc, 1)
    IDEXW(0x7f, J, jcc, 1)

    IDEXW(0x80, I2E, gp1, 1)
    IDEX(0x81, I2E, gp1)
    IDEX(0x83, SI2E, gp1)
    //0x84-0x85 test 0x86 xchg
    IDEXW(0x84, G2E, test, 1)
    IDEX(0x85, G2E, test)
    IDEX(0x86, G2E, xchg)
    IDEXW(0x88, mov_G2E, mov, 1)
    IDEX(0x89, mov_G2E, mov)
    IDEXW(0x8a, mov_E2G, mov, 1)
    IDEX(0x8b, mov_E2G, mov)
    IDEX(0x8d, lea_M2G, lea)

    EX(0x90, nop)
    //0x91-0x97 xchg
    IDEX(0x91, xchg_a2r, xchg)
    IDEX(0x92, xchg_a2r, xchg)
    IDEX(0x93, xchg_a2r, xchg)
    IDEX(0x94, xchg_a2r, xchg)
    IDEX(0x95, xchg_a2r, xchg)
    IDEX(0x96, xchg_a2r, xchg)
    IDEX(0x97, xchg_a2r, xchg)
    //cbw and cwd
    EX(0x98, cwtl)
    EX(0x99, cltd)
    // a:direct addr o: offset address
    IDEXW(0xa0, O2a, mov, 1)
    IDEX(0xa1, O2a, mov)
    IDEXW(0xa2, a2O, mov, 1)
    IDEX(0xa3, a2O, mov)
    //[(E)SI] to ES:[(E)DI]
    EXW(0xa4, movsb, 1)
    EX(0xa5, movsb)
    //DEST := LeftSRC AND RightSRC;
    IDEXW(0xa8, I2a, test, 1)
    IDEX(0xa9, I2a, test)
    IDEXW(0xb0, mov_I2r, mov, 1)
    IDEXW(0xb1, mov_I2r, mov, 1)
    IDEXW(0xb2, mov_I2r, mov, 1)
    IDEXW(0xb3, mov_I2r, mov, 1)
    IDEXW(0xb4, mov_I2r, mov, 1)
    IDEXW(0xb5, mov_I2r, mov, 1)
    IDEXW(0xb6, mov_I2r, mov, 1)
    IDEXW(0xb7, mov_I2r, mov, 1)
    IDEX(0xb8, mov_I2r, mov)
    IDEX(0xb9, mov_I2r, mov)
    IDEX(0xba, mov_I2r, mov)
    IDEX(0xbb, mov_I2r, mov)
    IDEX(0xbc, mov_I2r, mov)
    IDEX(0xbd, mov_I2r, mov)
    IDEX(0xbe, mov_I2r, mov)
    IDEX(0xbf, mov_I2r, mov)

    IDEXW(0xc0, gp2_Ib2E, gp2, 1)
    IDEX(0xc1, gp2_Ib2E, gp2)
    EX(0xc3, ret)

    IDEXW(0xc6, mov_I2E, mov, 1)
    IDEX(0xc7, mov_I2E, mov)

    EX(0xc9, leave) //无需译码直接操作
    IDEXW(0xd0, gp2_1_E, gp2, 1)
    IDEX(0xd1, gp2_1_E, gp2)
    IDEXW(0xd2, gp2_cl2E, gp2, 1)
    IDEX(0xd3, gp2_cl2E, gp2)
    EX(0xd6, nemu_trap)
    //pa2.1
    IDEX(0xe8, J, call)
    IDEX(0xe9, J, jmp)
    IDEXW(0xeb, J, jmp, 1)
    IDEXW(0xf6, E, gp3, 1)
    IDEX(0xf7, E, gp3)
    IDEXW(0xfe, E, gp4, 1)
    IDEX(0xff, E, gp5)
  case 0x66:
    s->isa.is_operand_size_16 = true;
    goto again; //0x66要改变宽度
  default:
    exec_inv(s);
  }
}

vaddr_t isa_exec_once()
{
  DecodeExecState s; //注意所有后续用到的s都是来自这个
  s.is_jmp = 0;
  s.isa = (ISADecodeInfo){0}; //isa相关信息初始为空
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s); //重点！！！
  update_pc(&s);

  return s.seq_pc;
}
