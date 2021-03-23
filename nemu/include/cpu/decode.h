#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include <isa.h>

#define OP_STR_SIZE 40
enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

typedef struct {
  uint32_t type;
  int width;
  union {
    uint32_t reg;
    word_t imm;
    sword_t simm;
  };
  rtlreg_t *preg;//用于提高性能
  rtlreg_t val;
  char str[OP_STR_SIZE];
} Operand;

//存放执行指令过程中的译码和执行信息
typedef struct {
  uint32_t opcode;
  vaddr_t seq_pc;  // sequential pc 当前pc
  uint32_t is_jmp; 
  vaddr_t jmp_pc;
  Operand src1, dest, src2; //源操作数和目的操作数
  int width;
  rtlreg_t tmp_reg[5];
  ISADecodeInfo isa; //isa相关信息
} DecodeExecState;

//译码辅助，获得操作对象
#define def_DHelper(name) void concat(decode_, name) (DecodeExecState *s)

#ifdef DEBUG
#define print_Dop(...) snprintf(__VA_ARGS__)
#else
#define print_Dop(...)
#endif

#endif
