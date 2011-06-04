
#ifndef	SPI_H
#define	SPI_H


#define DDR_CS      DDRB
#define PORT_CS     PORTB
#define P_CS        2

#define DDR_SPI     DDRB
#define PORT_SPI    PORTB
#define P_MISO      4
#define P_MOSI      3
#define P_SCK       5


extern uint8_t spi_putc(uint8_t data);
extern void spi_init(void);


#endif






