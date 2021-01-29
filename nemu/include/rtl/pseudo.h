#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz); //rz零寄存器，值总为0
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1 异或1等价于取反
  rtl_xori(s, dest, src1,-1);
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- -src1
  rtl_sub(s, dest, rz,src1);
}

//符号位拓展
static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 8 - 1) .. 0])
  switch(width){
    case 1: *dest = ((*src1 >> 7  & 1) ? *src1 | 0xFFFFFF00 : *src1 & 0x000000FF); break;
    case 2: *dest = ((*src1 >> 15 & 1) ? *src1 | 0xFFFF0000 : *src1 & 0x0000FFFF); break;
    case 4: *dest = *src1;                                                         break;
    default: assert(0);
  }
}

//0拓展
static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
  switch(width){
    case 1 : *dest = (uint32_t)(uint8_t)(*src1) ;break;
    case 2 : *dest = (uint32_t)(uint16_t)(*src1);break;
    case 4 : *dest = (uint32_t)(*src1);          break;
  }
}

//获取符号位
static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
  if(width == 4) *dest = *src1>>31;
  else if(width == 2) {rtl_andi(s,dest,src1,0xFFFF); *dest = *dest>>15;}
  else if(width == 1) {rtl_andi(s,dest,src1,0xFF);   *dest = *dest>>7; }
  else assert(0);
}

#endif
