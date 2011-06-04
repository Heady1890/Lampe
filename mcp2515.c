
/* INCLUDES */
/*#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm_generator.h"
#include "general.h"
#include "spi.h"

void mcp2515_write_register( uint8_t adress, uint8_t data )
{
    // /CS des MCP2515 auf Low ziehen
    PORT_CS &= ~(1<<P_CS);
    
    spi_putc(SPI_WRITE);
    spi_putc(adress);
    spi_putc(data);
    
    // /CS Leitung wieder freigeben
    PORT_CS |= (1<<P_CS);
}


uint8_t mcp2515_read_register(uint8_t adress)
{
    uint8_t data;
    
    // /CS des MCP2515 auf Low ziehen
    PORT_CS &= ~(1<<P_CS);
    
    spi_putc(SPI_READ);
    spi_putc(adress);
    
    data = spi_putc(0xff);  
    
    // /CS Leitung wieder freigeben
    PORT_CS |= (1<<P_CS);
    
    return data;
}*/
