#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "gpio.c"

#include "spi.h"
#include "spi.c"

#define REG_DECODE        0x09                        // "decode mode" register
#define REG_INTENSITY     0x0a                        // "intensity" register
#define REG_SCAN_LIMIT    0x0b                        // "scan limit" register
#define REG_SHUTDOWN      0x0c                        // "shutdown" register
#define REG_DISPLAY_TEST  0x0f                        // "display test" register

#define INTENSITY_MIN     0x00                        // minimum display intensity
#define INTENSITY_MAX     0x0f                        // maximum display intensity


#define MAX7219_BUILD_WORD(addr, cmd)     (uint16_t)( ( addr << 8 ) | cmd )
#define MAX7219_GPIO_DOUT_PIN             2
#define MAX7219_GPIO_CS_PIN               1
#define MAX7219_GPIO_CLK_PIN              0
#define DISPLAY_SIZE                      4


#define _delay(_ms)                       vTaskDelay( _ms / portTICK_PERIOD_MS );

typedef struct _D
{
    uint8_t Addr;
    uint8_t Data;
}Digit_t;

Digit_t Matrix[DISPLAY_SIZE];


void spi_soft_write( uint8_t _data )
{
  gpio_set_bit( GPIO1, 0, 1 );
  for (uint8_t i = 0; i < 8 ; i++)
  {
    if ( _data & 0x80 )
    {
      gpio_set_bit( GPIO2, 1, 0 );
    }
    else
    {
      gpio_set_bit( GPIO2, 0, 0 );
    }
    gpio_set_bit( GPIO0, 0, 0 );
    _delay(1);
    gpio_set_bit( GPIO0, 1, 0 );

    _data <<= 1;
  }
  gpio_set_bit( GPIO1, 1, 0 );
  _delay(5);
}

void max7219_write( uint8_t* _data, uint8_t _length )
{
  spi_write_bytes( _data, _length );
}

void max7219_shutdown_enable( void )
{
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_SHUTDOWN;
    *pData++ = 0;
  } 
  max7219_write( buffer, DISPLAY_SIZE  * 2 );  
}

void max7219_shutdown_disable( void )
{
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_SHUTDOWN;
    *pData++ = 1;
  } 
  max7219_write( buffer, DISPLAY_SIZE  * 2 );                   
}

void max7219_display_test_enable( void )
{
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_DISPLAY_TEST;
    *pData++ = 1;
  } 
  max7219_write( buffer, DISPLAY_SIZE  * 2 );               
}

void max7219_display_test_disable( void )
{ 
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_DISPLAY_TEST;
    *pData++ = 0;
  } 
  max7219_write( buffer, DISPLAY_SIZE  * 2 );            
}

void max7219_brightness( uint8_t brightness )
{
  brightness &= 0x0f; 

  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_INTENSITY;
    *pData++ = brightness;
  } 
  max7219_write( buffer, DISPLAY_SIZE  * 2 );   
}

void max7219_set_pixel( uint16_t x )
{
    Matrix[3-((x / 8) % 4)].Addr = (x / 32) + 1;
    Matrix[3-((x / 8) % 4)].Data = 1 << (x % 8);
}

void max7219_set_scan_limit( uint8_t _limit )
{                              
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_SCAN_LIMIT;
    *pData++ = _limit;
  } 
  spi_write_bytes( buffer, (DISPLAY_SIZE * 2) );  
}

void max7219_set_decode( uint8_t _decode )
{                              
  uint8_t *pData, buffer[ DISPLAY_SIZE * 2 ];
  pData = buffer; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *pData++ = REG_DECODE;
    *pData++ = _decode;
  } 
  spi_write_bytes( buffer, (DISPLAY_SIZE * 2) );  
}





void app_main(void)
{
    gpio_pin_map_to_peripheral(GPIO6, 64, 0 ); // miso
    gpio_pin_map_to_peripheral(GPIO2, 65, 0 ); // mosi
    gpio_pin_map_to_peripheral(GPIO1, 68, 0 ); // cs
    gpio_pin_map_to_peripheral(GPIO0, 63, 0 ); // clk

    spi_config_pins( GPIO2, GPIO6, GPIO0, GPIO1, 1);
    spi_start_bus( 1000000, SPI_DATA_MODE_0, SPI_BIT_ORDER_MSB );

     
    max7219_set_scan_limit(7);
    max7219_set_decode(0x00);                                          
    max7219_display_test_disable();                                                         
    max7219_brightness( 0x01 ); 

    while (1)
    {    
        //uint8_t buff[] = {1,2,3,4};
        //spi_write_bytes( buff, 4 );

        //uint8_t buff[] = {1,2,3,4};
        //spi_write_bytes( buff, 4 );

        for (uint8_t i = 0; i < 10; i++)
        {
          spi_write_byte(i);
        }
        
    }

}
