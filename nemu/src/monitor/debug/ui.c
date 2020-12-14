#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <memory/vaddr.h>
#include <isa/x86.h>
#include "expr.h"


extern void isa_reg_display(void);

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
//PA1.1
static int cmd_step(char *args);

static int cmd_scan(char *args);

static int cmd_watch(char *args);

static int cmd_delW(char *args);

// static int cmd_t(char *args);


// static int cmd_t(char *args){
//   bool flag = false;
//   expr(args, &flag);
//   return 0;
// }


static int cmd_step(char *args){
  char *arg = strtok(NULL, " ");

  if(!arg){
    cpu_exec(1);
  }
  else{
    bool flag = false;
    word_t num = expr(arg,&flag);
    cpu_exec(num);
  }
  return 0;
}

static int cmd_info(char *args){
  if(args[0] == 'r' ||args[0] == 'R'){
    isa_reg_display();
  }
  else if(args[0]=='w' || args[0]=='W'){
    print_wp();
  }
  else
  {
    printf("Unknown command, pls choose froe [r/R] [w/W]]\n");
  }
  
  return 0;
}

//TODO
static int cmd_scan(char *args){
  
  int num,addr;
  sscanf(args, "%d 0x%x", &num,&addr);

  //一次读32bit
  int i,j;
  //https://www.cnblogs.com/VIPler/category/658927.html
  for(i=0;i<num;i++){
    printf("0x%x   :  ",addr);
    for(j=0;j<4;j++){
      word_t data = vaddr_read(addr,1);
      printf("%02x ",data);
      addr++;
    }
      printf("\n");
  }
  return 0;
}

//什么逻辑？
static int cmd_watch(char *args){

  char *arg = strtok(NULL, " ");
  if(arg == NULL) return 0;

  WP * wp = new_wp();
  memset(wp->expr_str,0,sizeof(wp->expr_str));
  strcpy(wp->expr_str,arg);

  bool success = true;
  wp->value = expr(arg,&success);

  if(!success){
		printf("set watchpoint failed. Please check your exprssion!\n");
		free_wp(wp);
	}

  wp->hit = 0;
  return 0;
}

static int cmd_delW(char *args){
  char *arg = strtok(NULL, " ");
  if(!arg){
    printf("NO. of watchpoint can't be empty\n");
    return -1; 
  }
  int n;
  sscanf(arg,"%d",&n);
  bool success = del_wp(n);
  if(!success){
    printf("del watchpoint failed\n");
  }
  else{
    printf("del watchpoint %d success\n",n);
  }
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "s", "Exec one step",cmd_step},
  { "info","Info of registers or watchpoints",cmd_info},
  { "x", "Scan memory",cmd_scan},
  { "w", "set watchpoint",cmd_watch},
  { "d", "delete watchpoint",cmd_delW},
  //{ "test","test make_token",cmd_t},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) { //默认false
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);
    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1; //+1是因为由空格
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) { //如果第一个空格前的内容得以匹配，则调用相应处理函数
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }  //到达循环结尾，说明命令未知
  }
  
}
