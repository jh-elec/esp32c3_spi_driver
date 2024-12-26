
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

void gpio_set_io_mux(const gpio_t _pin, const uint8_t _func, const GPIO_IO_t _io )
{
  REG_SET_BIT( IO_MUX_GPIOn_REG( _pin ), ( _func << IO_MUX_GPIOn_MCU_SEL_bp ) ); 
  REG_CLR_BIT( GPIO_FUNCx_IN_SEL_CFG_REG( _pin ), GPIO_SIGx_IN_SEL_bm );

  switch ( _io )
  {
    case GPIO_IO_OUTPUT:
      REG_CLR_BIT( IO_MUX_GPIOn_REG( _pin ), ( IO_MUX_GPIOn_FUN_IE_bm | IO_MUX_GPIOn_FUN_WPU_bm ) ); 
    break;
    case GPIO_IO_INPUT:
      REG_SET_BIT( IO_MUX_GPIOn_REG( _pin ), IO_MUX_GPIOn_FUN_IE_bm ); 
    break; 
  }
}

void gpio_config_output( const gpio_t _pin )
{
  REG_SET_BIT( GPIO_ENABLE_W1TS_REG, 1 << _pin );
}

void gpio_set_high( const gpio_t _pin  )
{
  REG_SET_BIT( GPIO_OUT_W1TS_REG, 1 << _pin );
}

void gpio_set_low( const gpio_t _pin )
{
  REG_SET_BIT( GPIO_OUT_W1TC_REG, 1 << _pin );
}

void gpio_set_extendet( const gpio_t _pin, const uint8_t _level, const GPIO_INV_SIGNAL_t _inv )
{

  REG_SET_BIT(GPIO_ENABLE_REG, 1 << _pin );
  REG_SET_BIT( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ) , 0x80 | GPIO_FUNCx_OEN_SEL_bm );

  switch ( _inv )
  {
    case GPIO_INV_SIGNAL_DISABLE:
      REG_CLR_BIT( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ),  ( _inv << GPIO_FUNCx_OUT_INV_SEL_bp ) );
    break;

    case GPIO_INV_SIGNAL_ENABLE:
      REG_SET_BIT( GPIO_FUNCx_OUT_SEL_CNFG_REG( _pin ),  ( _inv << GPIO_FUNCx_OUT_INV_SEL_bp ) );
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

