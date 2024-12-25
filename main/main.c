#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "gpio.c"

#include "spi.h"
#include "spi.c"

#include "max7219.h"
#include "max7219.c"


#define _delay(_ms)                       vTaskDelay( _ms / portTICK_PERIOD_MS );


//#define SINGLE_CHIP

#ifdef SINGLE_CHIP
  #define GPIO_CLK    GPIO6
  #define GPIO_CS     GPIO10
  #define GPIO_MOSI   GPIO7
  #define GPIO_MISO   GPIO2
#else 
  #define GPIO_CLK    GPIO0
  #define GPIO_CS     GPIO1
  #define GPIO_MOSI   GPIO2
  #define GPIO_MISO   GPIO3
#endif 




void app_main(void)
{

  #ifdef SINGLE_CHIP
    gpio_set_io_mux(GPIO_MISO, 2, GPIO_IO_INPUT );
    gpio_set_io_mux(GPIO_MOSI, 2, GPIO_IO_OUTPUT );
    gpio_set_io_mux(GPIO_CLK, 2, GPIO_IO_OUTPUT);
    gpio_set_io_mux(GPIO_CS, 2, GPIO_IO_OUTPUT );
  #else
    gpio_pin_map_to_peripheral(GPIO_MISO, 64, 0 ); // miso FSPIQ
    gpio_pin_map_to_peripheral(GPIO_MOSI, 65, 0 ); // mosi FSPID
    //gpio_pin_map_to_peripheral(GPIO_CS, 68, 0 ); // cs FSPICS0
    gpio_pin_map_to_peripheral(GPIO_CLK, 63, 1 ); // clk FSPICLK
 #endif

    spi_start_bus( 1000000, SPI_DATA_MODE_0, SPI_BIT_ORDER_MSB );


    max7219_shutdown_enable();
  	max7219_shutdown_disable();

    max7219_set_scan_limit(0x07);
    max7219_set_decode(0x00);                                          
    max7219_display_test_disable();                                                         
    max7219_brightness( 0x01 ); 
    max7219_clear_display();

    while (1)
    { 
      max7219_clear_display();
      _delay(250);
      max7219_fill_display();
      _delay(10);
    } 
}
