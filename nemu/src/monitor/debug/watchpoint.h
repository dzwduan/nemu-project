#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;      //监视点序号
  struct watchpoint *next; 
 
  /* TODO: Add more members if necessary */
  
  char expr_value[32];
  uint32_t value;
} WP;
 

 

#endif
