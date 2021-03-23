#ifndef __DEVICE_MAP_H__
#define __DEVICE_MAP_H__

#include <monitor/difftest.h>

typedef void(*io_callback_t)(uint32_t, int, bool);
uint8_t* new_space(int size);

//用于映射 支持 端口/内存映射
typedef struct {
  char *name; 
  // we treat ioaddr_t as paddr_t here
  paddr_t low; //映射起始地址
  paddr_t high;
  uint8_t *space;//映射的目标空间
  io_callback_t callback; //回调函数
} IOMap;

static inline bool map_inside(IOMap *map, paddr_t addr) {
  //printf("map_inside  : 0x%x   map-high : 0x%x   addr : 0x%x\n",map->low,map->high,addr);
  return (addr >= map->low && addr <= map->high);
}

static inline int find_mapid_by_addr(IOMap *maps, int size, paddr_t addr) {
  int i;
  for (i = 0; i < size; i ++) {
    if (map_inside(maps + i, addr)) {
      difftest_skip_ref();
      return i;
    }
  }
  return -1;
}

void add_pio_map(char *name, ioaddr_t addr, uint8_t *space, int len, io_callback_t callback);
void add_mmio_map(char *name, paddr_t addr, uint8_t* space, int len, io_callback_t callback);

word_t map_read(paddr_t addr, int len, IOMap *map);
void map_write(paddr_t addr, word_t data, int len, IOMap *map);

#endif
