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

#include "max7219.h"
#include "spi.h"

/*!<-- Globale Variablen <--*/
/*****************************************************************/


/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/

/* map pins to peripheral
*	_pin: gpiox
*	_periphSignal: please see datasheet (signal list i/o)
*	_inv: can invert the output signal
*/
void max7219_write( uint8_t* _data, uint8_t _length )
{
  #ifdef SPI_USE_SW_CS0
    gpio_set_extendet( GPIO_CS, 1, 0 );
    gpio_set_extendet( GPIO_CS, 0, 0 );
  #endif

  for (size_t i = 0; i < _length; i++)
  {
    spi_write_byte( *_data++ );
  }

  #ifdef SPI_USE_SW_CS0
    gpio_set_extendet( GPIO_CS, 1, 0 );
  #endif
}

uint8_t* fill_n_matrix_ram ( uint8_t _digit, uint8_t* _ram, uint8_t _addr, uint8_t _data )
{
  uint8_t index =  ( 2 * _digit ) - 1;
  
  *(_ram + ( index - 1 )) = _addr;
  *(_ram + ( index )) = _data;
  
  return _ram; 
}

void max7219_shutdown_enable( void )
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_SHUTDOWN;
    *ptr_max7219_ram++ = 0;
  } 
  max7219_write( max7219_ram, DISPLAY_SIZE  * 2 );  
}

void max7219_shutdown_disable( void )
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_SHUTDOWN;
    *ptr_max7219_ram++ = 1;
  } 
  max7219_write( max7219_ram, DISPLAY_SIZE  * 2 );                   
}

void max7219_display_test_enable( void )
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_DISPLAY_TEST;
    *ptr_max7219_ram++ = 1;
  } 
  max7219_write( max7219_ram, DISPLAY_SIZE  * 2 );               
}

void max7219_display_test_disable( void )
{ 
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_DISPLAY_TEST;
    *ptr_max7219_ram++ = 0;
  } 
  max7219_write( max7219_ram, DISPLAY_SIZE  * 2 );            
}

void max7219_brightness( uint8_t brightness )
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  brightness &= 0x0f; 

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_INTENSITY;
    *ptr_max7219_ram++ = brightness;
  } 
  max7219_write( max7219_ram, DISPLAY_SIZE  * 2 );   
}

void max7219_set_scan_limit( uint8_t _limit )
{                              
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_SCAN_LIMIT;
    *ptr_max7219_ram++ = _limit;
  } 
  max7219_write( max7219_ram, (DISPLAY_SIZE * 2) );  
}

void max7219_set_decode( uint8_t _decode )
{                              
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;

  for (uint8_t i = 0; i < DISPLAY_SIZE; i++)
  {
    *ptr_max7219_ram++ = REG_DECODE;
    *ptr_max7219_ram++ = _decode;
  } 
  max7219_write( max7219_ram, (DISPLAY_SIZE * 2) );  
}

void max7219_clear_display() 
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;
  
  for ( uint8_t row = 0; row < 8; row++ )
  {
    ptr_max7219_ram = max7219_ram;
    for (uint8_t chip = 0 ; chip < 4 ; chip++ )
    {
      *ptr_max7219_ram++ = 1+row; // address
      *ptr_max7219_ram++ = 0x00; // data
    }   
    max7219_write( max7219_ram, DISPLAY_SIZE * 2 );
  }
}

void max7219_fill_display() 
{
  uint8_t max7219_ram[ DISPLAY_SIZE * 2 ] = {""};
  uint8_t *ptr_max7219_ram = max7219_ram;
  
  for ( uint8_t row = 0; row < 8; row++ )
  {
    ptr_max7219_ram = max7219_ram;
    for (uint8_t i = 0 ; i < 4 ; i++ )
    {
      *ptr_max7219_ram++ = 1+row; // address
      *ptr_max7219_ram++ = 0xFF; // data
    }   
    max7219_write( max7219_ram, DISPLAY_SIZE * 2 );
  }
}


/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/



// end of file

