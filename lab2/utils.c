#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  return OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return OK;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t byte = 0;
  sys_inb(port,&byte);
  *value = (uint8_t) byte;
  return OK;
}
