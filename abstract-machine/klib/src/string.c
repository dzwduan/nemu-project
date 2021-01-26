#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *eos = s;
  while(*eos++);
  return (eos-s-1);
}

//指针有效性，
//des 和 src 所指内存区域不可以重叠且 des 必须有足够的空间来容纳 src 的字符串
char *strcpy(char* dst,const char* src) {
  assert(dst && src);
  char * addr = dst;
  while((*dst++ = *src++)!='\0');

  return addr;
}

//strncpy的功能和strcpy相似，只是它复制时多了一个终止条件。
//即是未遇到原串的'\0’，如果已经复制了n个字符（n为提供的参数长度），复制同样会终止。
char* strncpy(char* dst, const char* src, size_t n) {
  
  char* d = dst;
  const char* s = src;

  do{
    if((*d++ = *s++)==0){
      //if len of dst > len of src 
      while(--n!=0){
        *d++ = 0;
      }
      break;
    }

  }while(--n);

  return (dst);
}

char* strcat(char* dst, const char* src) {
  assert(dst && src);
  char * addr = dst;
  while(*dst++ !='\0') ;
  while((*dst++ = *src++)!='\0');

  return addr;
}
/*
若s1==s2，返回零；
若s1>s2，返回正数；
若s1<s2，返回负数。
*/
int strcmp(const char* s1, const char* s2) {
  register const unsigned char * p1 = s1;
  register const unsigned char * p2 = s2;

  unsigned char * c1 ,*c2;

  do{
    c1 = (unsigned char) *p1++;
    c2 = (unsigned char) *p2++;
    if(c1=='\0')
      return c1 - c2;  
  }while(c1 == c2);

  return c1-c2;
}

int strncmp(const char* s1, const char* s2, size_t n) {

}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  return 0;
}

#endif
