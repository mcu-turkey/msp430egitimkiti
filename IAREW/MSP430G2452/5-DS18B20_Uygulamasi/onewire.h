
#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#define CONVERT_TMP     0x44
#define SKIPROM_CMD     0xCC
#define READ_SCRATCHPAD 0xBE

typedef struct 
{
  volatile unsigned char        *port_out;
  const volatile unsigned char  *port_in;
  volatile unsigned char        *port_dir;
  int pin;
} onewire_t;

void onewire_init (onewire_t *ow);
int  onewire_reset(onewire_t *ow);
void onewire_write_bit (onewire_t *ow, int bit);
int  onewire_read_bit  (onewire_t *ow);
void onewire_write_byte(onewire_t *ow, unsigned char byte);
unsigned char onewire_read_byte(onewire_t *ow);

#endif