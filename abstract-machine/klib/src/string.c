#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{ 
  const char *eos = s;
  while (*eos++)
    ;
  return (eos - s - 1);
}

//指针有效性，
//des 和 src 所指内存区域不可以重叠且 des 必须有足够的空间来容纳 src 的字符串
char *strcpy(char *dst, const char *src)
{
  char *addr = dst;
  while ((*dst++ = *src++) != '\0')
    ;

  return addr;
}

//strncpy的功能和strcpy相似，只是它复制时多了一个终止条件。
//即是未遇到原串的'\0’，如果已经复制了n个字符（n为提供的参数长度），复制同样会终止。
char *strncpy(char *dst, const char *src, size_t n)
{
  char *addr = dst;
  while(n-->0 && (*dst++ = *src++)!='\0');
  while(n-->0) *dst++=0;
  return addr;
}

char *strcat(char *dst, const char *src)
{
  printf("exec strcat \n");
  assert(dst && src);
  char *addr = dst;
  while (*dst++ != '\0')
    ;
  while ((*dst++ = *src++) != '\0')
    ;

  return addr;
}
/*
若s1==s2，返回零；
若s1>s2，返回正数；
若s1<s2，返回负数。
refer to glibc
*/
int strcmp(const char *s1, const char *s2)
{
  while (*s1)
  {
    if (*s1 == *s2)
    {
      s1++, s2++;
    }
    else
    {
      return *s1 - *s2;
    }
  }
  return *s1 - *s2;
}

/* Compare no more than N characters of S1 and S2,
   returning less than, equal to or greater than zero
   if S1 is lexicographically less than, equal to or
   greater than S2.  */
int strncmp(const char *s1, const char *s2, size_t n)
{

  assert(n>=0);

  while(n>0 && *s1 && *s1++==*s2++);
  if(n==0) return 0; 

  return *s1-*s2;
}

void *memset(void *v, int c, size_t n)
{
  char *dst = v;
  if (n >= 8)
  {
    size_t xlen;
    xlen = n >> 3;

    while (xlen--)
    {
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
      *dst++ = c;
    }
  }

  n = n & 0x7;
  while (n--)
    *dst++ = c;
  return 0;
}

//考虑区域重叠
void *memmove(void *dst, const void *src, size_t n)
{
  void *addr = dst;

  char *d = (char *)dst;
  char *s = (char *)src;
  //无重叠
  if ((dst >= src && src + n <= dst) || (dst <= src && dst + n <= src))
  {
    while (n--)
      *d++ = *s++;
  }
  //重叠部分，第一个可以合并
  else if (dst < src && dst + n > src)
  {
    while (n--)
      *d++ = *s++;
  }
  else if (dst > src && src + n > dst)
  {
    char *dend = (char *)(dst + n - 1);
    char *send = (char *)(src + n - 1);
    while (n--)
      *dend-- = *send--;
  }

  return addr;
}

//memcpy()可以假定两个内存区域之间没有重叠
void *memcpy(void *out, const void *in, size_t n)
{
  printf("exec memcpy \n");
  return memmove(out, in, n);
}

//The  memcmp()  function compares the first n bytes of the memory areas
int memcmp(const void *s1, const void *s2, size_t n)
{
 
  char *p1 = (char *)s1, *p2 = (char *)s2; 
  while (n-->0)
    {
      if (*p1 != *p2)
        return *p1 - *p2;
      p1++, p2++;
    }
  
  return 0;
}

#endif
