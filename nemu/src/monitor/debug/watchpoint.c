#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static int index;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


//pa1.3

//从free_中返回一个空闲wp结构
static inline WP * new_wp(char * args){

  bool success = true;
  int a = expr(args,&success);
  if(!success){
    printf("illegal expressiona!\n");
    return NULL;
  }

  WP * insert = free_;
  free_ = free_->next;
  insert->next = head;
  head = insert;
  head->value = a;
  memcpy(head->expr_value,args,strlen(args));

  return insert;
}

//wp归还到free_
static inline del_wp(int index){
  if(index>=0 && index<NR_WP){
    free_wp(&wp_pool[index]);
  }
  else{
    printf("Index %d out of range\n");
  }
}

void free_wp(WP *wp){
  if(!head || !wp) return;

  bool isFound = false;
  WP *present = head;

  if(head.)
}