#include <lcom/lcf.h>
#include <stdint.h>

#include "utils.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  return OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return OK;
}

#ifdef LAB3
    uint32_t sys_inb_counter = 0;
    int getCounter(){
      return sys_inb_counter;
    }
#endif

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t byte = 0;
  #define LAB3a
  sys_inb(port,&byte);
  *value = (uint8_t) byte;
    #ifdef LAB3
        ++sys_inb_counter;
    #endif
  return OK;
}

int16_t max_16(int16_t a, int16_t b){
  return (a < b ? b : a);
}

int16_t min_16(int16_t a, int16_t b){ 
  return (b < a ? b : a); 
}

