#ifndef SPI_H_
#define SPI_H_

/**
 * SMCLK divider arguments for spi_setspeed
 * change if your SMCLK isn't 16MHz
 */
#define SPI_250kHz 64 /* 16MHz/250000 */
#define SPI_400kHz 40 /* 16MHz/400000 */
#define SPI_1MHz   16 /* 16MHz/1MHz */
#define SPI_2MHz    8 /* 16MHz/2MHz */
#define SPI_4MHz    4 /* 16MHz/4MHz */
#define SPI_8MHz    2 /* 16MHz/8MHz */
#define SPI_16MHz   1 /* 16MHz/16Mhz */

#define SPI_DEF_SPEED SPI_4MHz

/**
 * utility macros for word argument
 */
#ifndef LOBYTE
#define LOBYTE(w) ((w)&0xFF)
#define HIBYTE(w) ((w)>>8)
#endif

void spi_initialize(void);
uint8_t spi_send(const uint8_t);
uint8_t spi_receive(void);
void spi_set_divisor(const uint16_t clkdivider);

#endif /*SPI_H_*/
