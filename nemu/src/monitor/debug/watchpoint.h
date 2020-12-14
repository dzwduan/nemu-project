#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;      //监视点序号
  struct watchpoint *next; 
 
  /* TODO: Add more members if necessary */
  
  char expr_str[32];
  uint32_t value;
  uint32_t hit;
} WP;
 

WP* new_wp();
void free_wp(WP *wp);

bool check_wp();

bool del_wp(int n);

void print_wp();

#endif
