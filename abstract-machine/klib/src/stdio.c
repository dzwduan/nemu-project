#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char buf[1024];

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
    int ret = vsprintf(buf, fmt, ap);
    va_end(ap);
    size_t i = 0;
    while (buf[i] != '\0') {
        putch(buf[i]);
        i++;
    }
    return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    int arg_int;
  char *arg_s;
  int len=0;
  int base=10;
  size_t pos=0;

  while(*fmt){
    switch(*fmt){
    case '%':
      ++fmt;
      switch(*fmt){
        case 'd': 
          base = 10;
          arg_int = va_arg(ap, int);
          len = itoa(arg_int,base,out+pos);
          pos+=len;
          break;
        case 'x':
          base = 16;
          arg_int = va_arg(ap, int);
          len = itoa(arg_int,base,out+pos);
          pos+=len;
          break;
        case 's': 
          arg_s = va_arg(ap,char *);
          strcat(out,arg_s);
          len = strlen(arg_s);
          pos+=len;
          break;
        case 'c':
          arg_int = va_arg(ap,int);
          out[pos] = arg_int+'0';
          pos++;
          break;
        default: break;
      }
    break;
    default: 

    out[pos++]=*fmt;
    break;
  }
  fmt++;
  }
  out[pos] = '\0';
  va_end(ap);
  return pos;
}


int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    assert(0);
    return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    assert(0);
    return 0;
}

#endif
