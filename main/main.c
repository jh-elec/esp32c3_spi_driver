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

typedef struct _D
{
    uint8_t Addr;
    uint8_t Data;
}Digit_t;

Digit_t Matrix[DISPLAY_SIZE];



void MAX7219_SendWord( uint8_t Addr, uint8_t Data )
{
  uint16_t Cmd = MAX7219_BUILD_WORD( Data, Addr  );
  
  gpio_set_bit( GPIO1 , 1, 0 );
  vTaskDelay(25 / portTICK_PERIOD_MS );
  gpio_set_bit( GPIO1 , 0, 0 );
  spi_write_word( Cmd );
  gpio_set_bit( GPIO1 , 1, 0 );

}

void MAX7219_Write_DBG ( void )
{
  for( uint8_t y = 0 ; y < 4 ; y++ )
  {
    for( uint8_t x = 0 ; x < 8 ; x ++ )
    {
        MAX7219_SendWord( Matrix[y].Addr, Matrix[x].Data );
    }
  }            
}

void MAX7219_ShutdownStart( void )
{
  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    Matrix[i].Addr = REG_SHUTDOWN;
    Matrix[i].Data = 0;   
  } 
  MAX7219_Write_DBG();         
}

void MAX7219_ShutdownStop( void )
{
  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    Matrix[i].Addr = REG_SHUTDOWN;
    Matrix[i].Data = 1;   
  }
  MAX7219_Write_DBG();                  
}

void MAX7219_DisplayTestStart( void )
{
  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    Matrix[i].Addr = REG_DISPLAY_TEST;
    Matrix[i].Data = 1;   
  }   
  MAX7219_Write_DBG();               
}

void MAX7219_DisplayTestStop( void )
{ 
  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    Matrix[i].Addr = REG_DISPLAY_TEST;
    Matrix[i].Data = 0;   
  } 
  MAX7219_Write_DBG();              
}

void MAX7219_SetBrightness( uint8_t brightness )
{
  brightness &= 0x0f;                                 
  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    Matrix[i].Addr = REG_INTENSITY;
    Matrix[i].Data = brightness;   
  }  
  MAX7219_Write_DBG(); 
}

void MAX7219_Clear( void )
{
  for (uint8_t y = 0; y < DISPLAY_SIZE; y++)
  {
    for ( uint8_t x = 1 ; x < 9 ; x++ )
    {
        Matrix[y].Addr = x;
        Matrix[y].Data = 0; 
    }      
  }  
  MAX7219_Write_DBG();   
}

void MAX7219_Screensaver( void )
{
  uint8_t cnt = 1;

  while( cnt < 8 )
  {
    for ( uint8_t x = 0 ; x < 31 ; x++ )
    {
      Matrix[3-((x / 8) % 4)].Addr = cnt;
      Matrix[3-((x / 8) % 4)].Data = 1 << (x % 8);
      MAX7219_Write_DBG();
      vTaskDelay((30 - x) / portTICK_PERIOD_MS);
      Matrix[3-((x / 8) % 4)].Data = 0;
      MAX7219_Write_DBG();
    }
    cnt++;

    for ( uint8_t x = 0 ; x < 31 ; x++ )
    {
      Matrix[3-((31-x / 8) % 4)].Addr = cnt;
      Matrix[3-((31-x / 8) % 4)].Data = 1 <<((31-x) % 8);
      MAX7219_Write_DBG();
      vTaskDelay((30 - x)/portTICK_PERIOD_MS);
      Matrix[3-((31-x / 8) % 4)].Data = 0;
      MAX7219_Write_DBG();  
    }
    cnt++;  
  }
}

void MAX7219_SetPixel( uint16_t x )
{
    MAX7219_Clear();
    Matrix[3-((x / 8) % 4)].Addr = (x / 32) + 1;
    Matrix[3-((x / 8) % 4)].Data = 1 << (x % 8);
    MAX7219_Write_DBG();
}

void app_main(void)
{
    gpio_pin_map_to_peripheral(GPIO6, 64, 0 ); // miso
    gpio_pin_map_to_peripheral(GPIO2, 65, 0 ); // mosi
    //gpio_pin_map_to_peripheral(GPIO1, 68, 0 ); // cs
    gpio_pin_map_to_peripheral(GPIO0, 63, 0 ); // clk

    spi_config_pins( GPIO2, GPIO6, GPIO0, GPIO1, 1);
    spi_start_bus( 1000000, SPI_DATA_MODE_0, SPI_BIT_ORDER_MSB );

    MAX7219_SendWord(REG_SCAN_LIMIT, 7);                  
    MAX7219_SendWord(REG_DECODE, 0x00);                    
    MAX7219_ShutdownStop();                             
    MAX7219_DisplayTestStop();                          
    MAX7219_Clear();                                    
    MAX7219_SetBrightness(0x01);     

    while (1)
    {    
        MAX7219_Clear();
    }

}