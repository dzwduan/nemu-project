void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "monitor/debug/expr.h"

extern word_t expr(char *e, bool *success);

int test_expr(char * filename){
  FILE * fp;
  char * p,buf[1024];
  int lineNum = 0;

  if((fp = fopen(filename,"r"))==NULL)
    return -1;

  //fgets从指定的流读取一行
  for(lineNum=0;fgets(buf,sizeof(buf),fp)!=NULL;lineNum++){
      if ((p = strchr(buf, '\n')) == NULL) {
            p = buf + strlen(buf);
        }
        if (p > buf && p[-1] == '\r') {
            --p;
        }
        *p = '\0';
        for (p = buf; *p != '\0'&& isspace((int)*p); ++p) {
            ;
        }
        if (*p == '\0') {
            continue;
        }
    
    printf("%s\n",p);


    char *arg1 = strtok(p, " ");
    char *arg2 = arg1+strlen(arg1)+1;

    //printf("LineNum : %d , arg1 : %s  ,arg2 : %s\n",lineNum,arg1,arg2);
    
    int num = strtol(arg1,NULL,10);
    
    bool succ;

    int predict = expr(arg2,&succ);

    //printf("result = %d , predict = %d\n",num,predict);
    if(predict == num)
      printf("PASS\n");
    else
    {
      printf("FAIL\n");
    }
    
  }
  fclose(fp);
  return 0;
}


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  init_monitor(argc, argv);
  printf("start test code\n");
  test_expr("/home/dzw/ics2020/nemu/src/input");
  printf("end test code\n");
  /* Start engine. */
  engine_start();


  return is_exit_status_bad();
}
