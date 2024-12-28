
/*************************************************************
*|
*|	\@file  	sysreg.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	esp32c3 sysreg
*|
*|	\@date		28/12/2024 first implementation
*|
*|	\@todo 		in progress
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	0.0.281224
*|
*|**************************************************************/

#ifndef __SYSREG_H__
#define __SYSREG_H__

#include <stdint.h>
#include <stdbool.h>


/*!<-- Defines <--*/
/*****************************************************************/

#define REG_WRITE(_r, _b)  		(*(volatile uint32_t*)(_r)) = (_b)      
#define REG_READ(_r)  			(*(volatile uint32_t*)(_r))      
#define REG_SET_BIT(_r, _b)  	(*(volatile uint32_t*)(_r)) = (*(volatile uint32_t*)(_r)) | (_b)                                          
#define REG_CLR_BIT(_r, _b)  	(*(volatile uint32_t*)(_r)) = (*(volatile uint32_t*)(_r)) & (~(_b))          


#define SYSREG_BASE                         0x600C0000 
#define SYSREG_REG(r)                       (SYSREG_BASE + r)

#define SYSTEM_PERIP_CLK_EN0_REG            SYSREG_REG(0x10)
#define SYSTEM_SPI2_CLK_EN_bp				6
#define SYSTEM_SPI2_CLK_EN_bm				BIT(SYSTEM_SPI2_CLK_EN_bp)

#define SYSTEM_PERIP_RST_EN0_REG            SYSREG_REG(0x18)
#define SYSTEM_SPI2_RST_bp					6
#define SYSTEM_SPI2_RST_bm					BIT(SYSTEM_SPI2_RST_bp)

#define SYSTEM_SYSCLK_CONF_REG              SYSREG_REG(0x58)
#define SYSTEM_SOC_CLK_SEL_bp               10
#define SYSTEM_SOC_CLK_SEL_bm               0xC00
#define SYSTEM_CLK_XTAL_FREQ_bp             12
#define SYSTEM_CLK_XTAL_FREQ_bm             0x7F000

#define SYSTEM_CPU_PER_CONF_REG             SYSREG_REG(0x08)
#define SYSTEM_PLL_FREQ_SEL_bp              2
#define SYSTEM_PLL_FREQ_SEL_bm              BIT(SYSTEM_PLL_FREQ_SEL_bp)
#define SYSTEM_CPUPERIOD_SEL_bp             0
#define SYSTEM_CPUPERIOD_SEL_bm             0x03


/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

/* for more information please see page 188 @datasheet for esp32c3 */

typedef enum cpu_clock_source_e
{
    CPU_CLK_SRC_XTAL_CLK,
    CPU_CLK_SRC_PLL_CLK_1,
    CPU_CLK_SRC_PLL_CLK_2,
    CPU_CLK_SRC_PLL_CLK_3,
    CPU_CLK_SRC_PLL_CLK_4,
    CPU_CLK_SRC_RC_FAST_CLK
}cpu_clock_source_t;

const uint8_t cpu_clock_source[][3] = 
{
                        //SYSTEM_SOC_CLK_SEL    |   SYSTEM_PLL_FREQ_SEL   |   SYSTEM_PERIOD_SEL
/* XTAL_CLK */           {   0                   ,   0                     ,   0               }, // CPU_CLK = XTAL_CLK/(SYSTEM_PRE_DIV_CNT + 1) - SYSTEM_PRE_DIV_CNT ranges from 0 ~ 1023. Default is 1
/* PLL_CLK_480MHz */     {   1                   ,   1                     ,   0               }, // CPU_CLK = PLL_CLK/6 - CPU_CLK frequency is 80 MHz
/* PLL_CLK_480MHz */     {   1                   ,   1                     ,   1               }, // CPU_CLK = PLL_CLK/3 - CPU_CLK frequency is 160 MHz
/* PLL_CLK_320MHz */     {   1                   ,   0                     ,   0               }, // CPU_CLK = PLL_CLK/4 - CPU_CLK frequency is 80 MHz
/* PLL_CLK_320MHz */     {   1                   ,   0                     ,   1               }, // CPU_CLK = PLL_CLK/2 - CPU_CLK frequency is 160 MHz
/* RC_FAST_CLK*/         {   2                   ,   0                     ,   0               }, // CPU_CLK = RC_FAST_CLK/(SYSTEM_PRE_DIV_CNT + 1) - SYSTEM_PRE_DIV_CNT ranges from 0 ~ 1023. Default is 1
};

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/


/* spi start bus
*	_clockDiv: config the prescaler for frequency
*	_dataMode: spi data mode (0..3)
*	_bitOrder: msb or lsb first
*/


/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SYSREG_H__


