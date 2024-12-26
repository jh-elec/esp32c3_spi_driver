
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

#ifndef __GPIO_H__
#define __GPIO_H__



/*!<-- Defines <--*/
/*****************************************************************/

#define BIT(_x)								(1 << _x)

#define GPIO_BASE                           0x60004000
#define GPIO_REG(r)                         (GPIO_BASE + r)

#define IO_MUX_BASE                         0x60009000
#define IO_MUX_REG(r)                       (IO_MUX_BASE + r)

#define GPIO_FUNCx_OUT_SEL_CNFG_REG(n)      GPIO_REG((0x0554 + (4*n)))
#define GPIO_FUNCx_OUT_SEL_bp               0
#define GPIO_FUNCx_OUT_SEL_bm               0xFF
#define GPIO_FUNCx_OUT_INV_SEL_bp           8
#define GPIO_FUNCx_OUT_INV_SEL_bm           BIT(GPIO_FUNCx_OUT_INV_SEL_bp)
#define GPIO_FUNCx_OEN_SEL_bp               9
#define GPIO_FUNCx_OEN_SEL_bm               BIT(GPIO_FUNCx_OEN_SEL_bp)
#define GPIO_FUNCx_OEN_INV_SEL_bp           10
#define GPIO_FUNCx_OEN_INV_SEL_bm           BIT(GPIO_FUNCx_OEN_INV_SEL_bp) 

#define GPIO_PINn_REG(n)					GPIO_REG((0x0074 + (4*n)))
#define GPIO_PINn_PAD_DRIVER_bp				2
#define GPIO_PINn_PAD_DRIVER_bm				BIT(GPIO_PINn_PAD_DRIVER_bp)

#define GPIO_FUNCx_IN_SEL_CFG_REG(n)		GPIO_REG((0x0554 + (4*n)))
#define GPIO_SIGx_IN_SEL_bp					6
#define GPIO_SIGx_IN_SEL_bm					BIT(GPIO_SIGx_IN_SEL_bp)

#define GPIO_BT_SELECT_REG                  GPIO_REG(0x00)
#define GPIO_OUT_REG                        GPIO_REG(0x04)
#define GPIO_OUT_W1TS_REG                   GPIO_REG(0x08)
#define GPIO_OUT_W1TC_REG                   GPIO_REG(0x0C)
#define GPIO_ENABLE_REG                     GPIO_REG(0x20)
#define GPIO_ENABLE_W1TS_REG                GPIO_REG(0x24)
#define GPIO_ENABLE_W1TC_REG                GPIO_REG(0x28)
#define GPIO_STRAP_REG                      GPIO_REG(0x38)
#define GPIO_IN_REG                         GPIO_REG(0x3C)
#define GPIO_STATUS_REG                     GPIO_REG(0x44)
#define GPIO_STATUS_W1TS_REG                GPIO_REG(0x48)
#define GPIO_STATUS_W1TC_REG                GPIO_REG(0x4C)
#define GPIO_PCPU_INT_REG                   GPIO_REG(0x5C)
#define GPIO_STATUS_NEXT_REG                GPIO_REG(0x4C) 
 
#define IO_MUX_GPIOn_REG(n)                 IO_MUX_REG((0x04 + (4*n)))
#define IO_MUX_GPIOn_MCU_IE_bp				4
#define IO_MUX_GPIOn_MCU_IE_bm				BIT(IO_MUX_GPIOn_MCU_IE_bp)
#define IO_MUX_GPIOn_MCU_SEL_bp				12
#define IO_MUX_GPIOn_MCU_SEL_bm				BIT(IO_MUX_GPIOn_MCU_SEL_bp)
#define IO_MUX_GPIOn_FUN_IE_bp				9
#define IO_MUX_GPIOn_FUN_IE_bm				BIT(IO_MUX_GPIOn_FUN_IE_bp)
#define IO_MUX_GPIOn_FUN_WPD_bp				7
#define IO_MUX_GPIOn_FUN_WPD_bm				BIT(IO_MUX_GPIOn_FUN_WPD_bp)
#define IO_MUX_GPIOn_FUN_WPU_bp				8
#define IO_MUX_GPIOn_FUN_WPU_bm				BIT(IO_MUX_GPIOn_FUN_WPU_bp)
#define IO_MUX_GPIOn_MCU_OE_bp				1
#define IO_MUX_GPIOn_MCU_OE_bm				BIT(IO_MUX_GPIOn_MCU_OE_bp)

#define REG_WRITE(_r, _b)  		(*(volatile uint32_t*)(_r)) = (_b)      
#define REG_READ(_r)  			(*(volatile uint32_t*)(_r))      
#define REG_SET_BIT(_r, _b)  	(*(volatile uint32_t*)(_r)) = (*(volatile uint32_t*)(_r)) | (_b)                                          
#define REG_CLR_BIT(_r, _b)  	(*(volatile uint32_t*)(_r)) = (*(volatile uint32_t*)(_r)) & (~(_b))                                        

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/
typedef enum GPIO_INV_SIGNAL_e
{
  GPIO_INV_SIGNAL_DISABLE,
  GPIO_INV_SIGNAL_ENABLE
}GPIO_INV_SIGNAL_t;

typedef enum GPIO_IO_e
{
	GPIO_IO_OUTPUT,
	GPIO_IO_INPUT
}GPIO_IO_t;

typedef enum GPIOn_e
{
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3,
	GPIO4,
	GPIO5,
	GPIO6,
	GPIO7,
	GPIO8,
	GPIO9,
	GPIO10,
	GPIO11,
	GPIO12,
	GPIO13,
	GPIO14,
	GPIO15,
	GPIO16,
	GPIO17,
	GPIO18,
	GPIO19,
	GPIO20,
	GPIO21
}gpio_t;

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/

/* map pins to peripheral
*	_pin: gpiox
*	_periphSignal: please see datasheet (signal list i/o)
*	_inv: can invert the output signal
*/
void gpio_pin_map_to_peripheral( const gpio_t _pin, uint8_t _periphSignal, const GPIO_INV_SIGNAL_t _inv );

/* configure io mux 
*	_pin: gpiox
*	_func: please see datasheet (io mux function)
*/
void gpio_set_io_mux(const gpio_t _pin, const uint8_t _func, const GPIO_IO_t _io);

/* configure gpio output via special gpio enable register
*	_pin: gpiox
*/
void gpio_config_output( const gpio_t _pin );

/* set gpiox to high level
*	_pin: gpiox
*/
void gpio_set_high( const gpio_t _pin  );

/* set gpiox to low level
*	_pin: gpiox
*/
void gpio_set_low( const gpio_t _pin );

/* configure io mux 
*	_pin: gpiox
*	_level: 0 = output "low", 1 = output "high"
*	_inv: 0 = not inverted, 1 = inverted
*/
void gpio_set_extendet( const gpio_t _pin, const uint8_t _level, const GPIO_INV_SIGNAL_t _inv );


void gpio_enable_output( const gpio_t _pin );

/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


#define DEBUG_LED_ON	gpio_set_bit( GPIO8, 0, 0 )
#define DEBUG_LED_OFF	gpio_set_bit( GPIO8, 1, 0 )


// end of file



#endif // __GPIO_H__


