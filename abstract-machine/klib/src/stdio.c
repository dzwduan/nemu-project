#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char out[1024];
//==========================helper function======================================
#define is_digit(x) ((x)>='0'&&(x)<='9')
//支持负数
int itoa(int num, int base,char * ret){

  int flag = 0;
  if(num<0) flag =1, num=-num;

  char buf[16];
  int i=0,j=0;
  while(num>0){
    buf[i++] = num%base + '0';
    num/=10;
  }

  if(flag == 1) buf[i++]='-';
  buf[i]='\0';

  for(j=i-1;j>=0;j--){
    if(is_digit(buf[i-j-1]) || buf[i-j-1]=='-')
      ret[j] = buf[i-j-1];
    else
      ret[j] = buf[i-j-1]-'9'+'a'-1;
  }
  ret[i] = '\0';
 
  return i;
}



int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = vsprintf(out,fmt,ap);
  va_end(ap);
  size_t i = 0;
    while (out[i] != '\0') {
        putch(out[i]);
        i++;
    }
  return ret;
}

//成功则返回写入的总字数
int vsprintf(char *out, const char *fmt, va_list ap) {
  int arg_int;
  char *arg_s;
  char arg_c;
  int len=0;
  int flag = 0;
  int base=10;
  size_t pos=0;

  while(*fmt++!='\0'){
    switch(*fmt){
      case '%':
        flag = 1;
        break;
      case 'c':
        if(flag == 1){ 
          arg_c = va_arg(ap,int);
          *out++ = arg_c;
          pos++;
        }
        flag = 0;
        break;
      case 's': 
        if(flag == 1){
          arg_s = va_arg(ap,char *);
          strcat(out,arg_s);
          len = strlen(arg_s);
          out+=len;
          pos+=len;
        }
        flag = 0;
        break;
      case 'd': 
        if(flag == 1){
          base = 10;
          arg_int = va_arg(ap,int);
          len = itoa(arg_int,base,out);
          out+=len;
          pos+=len;
        }
        flag = 0;
        break;
      case 'x': 
        if(flag == 1){
          base=16;
          arg_int = va_arg(ap,int);
          len = itoa(arg_int,base,out);
          out+=len;
          pos+=len;
        }
        flag = 0;
        break;
      default: 
        *out++ = *fmt;
        pos++;
        break;
    }
  }
  *out = '\0';
  va_end(ap);
  return pos;
}

int sprintf(char *out, const char *fmt, ...) {
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
