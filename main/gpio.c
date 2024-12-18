
/*************************************************************
*|
*|	\@file  	gpio.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	esp32c3 gpio driver
*|
*|	\@date		17/12/2024 first implementation
*|
*|	\@todo 		in progress
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	0.0.171224
*|
*|**************************************************************/

#include <stdint.h>
#include "gpio.h"



void gpio_pin_map_to_peripheral( const gpio_t _pin, uint8_t _periphSignal, const GPIO_INV_SIGNAL_t _inv )
{
  uint8_t periphSignal = 0x80;

  if ( _periphSignal != 0 ) periphSignal = _periphSignal;

    REG_WRITE( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ), ( _inv << 8 ) | ( periphSignal ));
}

void gpio_set_io_mux(const gpio_t _pin, uint8_t _func )
{
  REG_SET_BIT( IO_MUX_GPIOn_REG( _pin ), ( _func << 12 ) );  
}

void gpio_set_bit( const gpio_t _pin, const uint8_t _level, uint8_t _inv )
{
  REG_SET_BIT(GPIO_ENABLE_REG, 1 << _pin );
  
  switch ( _inv )
  {
    case 0:
      REG_CLR_BIT( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ),  ( _inv << GPIO_FUNCx_OUT_INV_SEL_bp ) );
    break;

    case 1:
      REG_SET_BIT( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ),  ( _inv << GPIO_FUNCx_OUT_INV_SEL_bp ) );
    break; 

    default:
    break;
  }

  switch ( _level )
  {
    case 0:
      REG_CLR_BIT( GPIO_OUT_REG, 1 << _pin );
    break;

    case 1:
      REG_SET_BIT( GPIO_OUT_REG, 1 << _pin );
    break;
  }
}