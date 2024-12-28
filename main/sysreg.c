
/*************************************************************
*|
*|	\@file  	spi.c
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	esp32c3 spi driver
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
#include <string.h>

#include "sysreg.h"



void sys_set_soc_clk_sel( uint8_t _sel )
{
    REG_SET_BIT( SYSTEM_SYSCLK_CONF_REG, ( _sel & 0x03 ) << SYSTEM_SOC_CLK_SEL_bp );
}

void sys_set_pll_clock_frequency( uint8_t _val )
{
    REG_SET_BIT( SYSTEM_CPU_PER_CONF_REG, ( _val & 0x01 ) << SYSTEM_PLL_FREQ_SEL_bp );
}

void sys_set_cpu_period_select( uint8_t _val )
{
    REG_SET_BIT( SYSTEM_CPU_PER_CONF_REG, ( _val & 0x03 ) << SYSTEM_CPUPERIOD_SEL_bp );
}

void sys_set_cpu_clock_frequency( const cpu_clock_source_t _src )
{
    sys_set_soc_clk_sel         ( cpu_clock_source[ _src ][0] );
    sys_set_pll_clock_frequency ( cpu_clock_source[ _src ][1] );
    sys_set_cpu_period_select   ( cpu_clock_source[ _src ][2] );
}

uint8_t sys_get_cpu_period_select()
{
   return ( REG_READ( SYSTEM_CPU_PER_CONF_REG ) & SYSTEM_CPUPERIOD_SEL_bm ) >> SYSTEM_CPUPERIOD_SEL_bp;    
}

uint8_t sys_get_pll_clock_frequency()
{
   return ( REG_READ( SYSTEM_CPU_PER_CONF_REG ) & SYSTEM_PLL_FREQ_SEL_bm ) >> SYSTEM_PLL_FREQ_SEL_bp;
}

uint8_t sys_get_soc_clk_sel()
{
  return ( ( REG_READ( SYSTEM_SYSCLK_CONF_REG ) & SYSTEM_SOC_CLK_SEL_bm ) >> SYSTEM_SOC_CLK_SEL_bp );
}

uint8_t sys_get_xtal_clk_freq()
{
  return ( ( REG_READ( SYSTEM_SYSCLK_CONF_REG ) & SYSTEM_CLK_XTAL_FREQ_bm ) >> SYSTEM_CLK_XTAL_FREQ_bp );
}



