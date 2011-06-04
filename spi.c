
/* INCLUDES */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm_generator.h"
#include "general.h"
#include "spi.h"


void spi_init(void)
{
    // Aktivieren der Pins für das SPI Interface
    DDR_SPI  |= (1<<P_SCK)|(1<<P_MOSI);
    PORT_SPI &= ~((1<<P_SCK)|(1<<P_MOSI)|(1<<P_MISO));
    
    DDR_CS   |= (1<<P_CS);
    PORT_CS  |= (1<<P_CS);
    
    // Aktivieren des SPI Master Interfaces, fosc = fclk / 2
    SPCR = (1<<SPE)|(1<<MSTR);
    SPSR = (1<<SPI2X);
}

uint8_t spi_putc( uint8_t data )
{
    // Sendet ein Byte
    SPDR = data;
    
    // Wartet bis Byte gesendet wurde
    while( !( SPSR & (1<<SPIF) ) )
        ;
    
    return SPDR;
}
