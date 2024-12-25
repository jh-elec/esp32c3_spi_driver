/*************************************************************
*|
*|	\@file  	max7219.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	max7219 spi driver
*|
*|	\@date		25/12/2024 first implementation
*|
*|	\@todo 		in progress
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	0.0.251224
*|
*|**************************************************************/

#ifndef __MAX7219_H__
#define __MAX7219_H__

#include <stdio.h>
#include <stdint.h>


/*!<-- Defines <--*/
/*****************************************************************/
#define DISPLAY_SIZE                      4


#define MAX7219_BUILD_WORD(addr, cmd)     (uint16_t)( ( addr << 8 ) | cmd )

#define REG_DECODE              0x09 // "decode mode" register
#define REG_INTENSITY           0x0a // "intensity" register
#define REG_SCAN_LIMIT          0x0b // "scan limit" register
#define REG_SHUTDOWN            0x0c // "shutdown" register
#define REG_DISPLAY_TEST        0x0f // "display test" register

#define INTENSITY_MIN           0x00 // minimum display intensity
#define INTENSITY_MAX           0x0f // maximum display intensity

  #define GPIO_CLK              GPIO0
  #define GPIO_CS               GPIO1
  #define GPIO_MOSI             GPIO2
  #define GPIO_MISO             GPIO3              
/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/


/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/

/* write bytes to max7219 via spi
*	_data: pointer to data buffer
*	_length: length of byte to transmit
*/
void max7219_write( uint8_t* _data, uint8_t _length );

/* write commando and data for _digit in _ram
*	_digit: num of max7219 which want to be write
*	_ram: pointer to ram how the data to be safe
*   _addr: address of max7219 register
*   _data: data to be write
*/
uint8_t* fill_n_matrix_ram ( uint8_t _digit, uint8_t* _ram, uint8_t _addr, uint8_t _data );

/* enable sleep mode for all max7219*/
void max7219_shutdown_enable( void );

/* disable sleep mode for all max7219*/
void max7219_shutdown_disable( void );

/* enable self test for all max7219*/
void max7219_display_test_enable( void );

/* disable self test for all max7219*/
void max7219_display_test_disable( void );

/* set bright for all max7219*/
void max7219_brightness( uint8_t brightness );

/* set scan limit for all max7219*/
void max7219_set_scan_limit( uint8_t _limit );

/* set decode mode for all max7219*/
void max7219_set_decode( uint8_t _decode );

/* clear display of all max7219*/
void max7219_clear_display();

/* fill display for all max7219*/
void max7219_fill_display();

/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/



// end of file



#endif // __MAX7219_H__


