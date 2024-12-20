
/*************************************************************
*|
*|	\@file  	spi.h
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

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/*!<-- Defines <--*/
/*****************************************************************/
#define SPI2_BASE                           0x60024000
#define SPI2_REG(r)                         (SPI2_BASE + r)

/* spi command register*/
#define SPI_CMD_REG                         SPI2_REG(0x00)
#define SPI_UPDATE_bp                       23
#define SPI_UPDATE_bm                       BIT(SPI_UPDATE_bp)
#define SPI_USR_bp                          24
#define SPI_USR_bm                          BIT(SPI_USR_bp)

/* spi address register */
#define SPI_ADDR_REG                        SPI2_REG(0x04)

#define SPI_USER_REG                        SPI2_REG(0x10)
#define SPI_USR_COMMAND_bp                  31
#define SPI_USR_COMMAND_bm                  BIT(SPI_USR_bp)
#define SPI_USR_ADDR_bp                     30
#define SPI_USR_ADDR_bm                     BIT(SPI_USR_ADDR_bp)
#define SPI_USR_DUMMY_bp                    29
#define SPI_USR_DUMMY_bm                    BIT(SPI_USR_DUMMY_bp)
#define SPI_DOUTDIN_bp                      0
#define SPI_DOUTDIN_bm                      BIT(SPI_DOUTDIN_bp)
#define SPI_USR_MOSI_bp                     27
#define SPI_USR_MOSI_bm                     BIT(SPI_USR_MOSI_bp)
#define SPI_USR_MISO_bp                     28
#define SPI_USR_MISO_bm                     BIT(SPI_USR_MISO_bp)
#define SPI_USR_MOSI_HIGHPART_bp            25
#define SPI_USR_MOSI_HIGHPART_bm            BIT(SPI_USR_MOSI_HIGHPART_bp)
#define SPI_CK_OUT_EDGE_bp					9
#define SPI_CK_OUT_EDGE_bm					BIT(SPI_CK_OUT_EDGE_bp)
#define SPI_USR_CONF_NXT_bp					15
#define SPI_USR_CONF_NXT_bm					BIT(SPI_USR_CONF_NXT_bp)

#define SPI_USER1_REG                       SPI2_REG(0x14)

#define SPI_USER2_REG                       SPI2_REG(0x18)
#define SPI_USER2_USR_COMMAND_VALUE_bm      0x0000FFFF
#define SPI_USER2_USR_COMMAND_BITLEN_bm     0xF0000000    

/* register for data to be send or received in non highpart mode */
#define SPI_Wn_REG(n)                       SPI2_REG((0x98+(n*4)))

/* data bit length to trasmit in master mode */
#define SPI_MS_DLEN_REG                     SPI2_REG(0x1C)

/* slave configuration register */
#define SPI_SLAVE_REG                       SPI2_REG(0xE0)
#define SPI_SLAVE_MODE_bp                   26
#define SPI_SLAVE_MODE_bm                   BIT(SPI_SLAVE_MODE_bp)


/* dma configuration register */
#define SPI_DMA_CONF_REG                    SPI2_REG(0x30)

#define SPI_DMA_AFIFO_RST_bp                31
#define SPI_DMA_AFIFO_RST_bm                BIT(SPI_DMA_AFIFO_RST_bp)

#define SPI_BUF_AFIFO_RST_bp                30
#define SPI_BUF_AFIFO_RST_bm                BIT(SPI_BUF_AFIFO_RST_bp)

#define SPI_RX_AFIFO_RST_bp                 29
#define SPI_RX_AFIFO_RST_bm                 BIT(SPI_RX_AFIFO_RST_bp)

#define SPI_DMA_RX_ENA_bp                   27
#define SPI_DMA_RX_ENA_bm                   BIT(SPI_DMA_RX_ENA_bp)

#define SPI_DMA_TX_ENA_bp                   28
#define SPI_DMA_TX_ENA_bm                   BIT(SPI_DMA_TX_ENA_bp)


#define SPI_MISC_REG                        SPI2_REG(0x20)
#define SPI_CK_IDLE_EDGE_bp					29
#define SPI_CK_IDLE_EDGE_bm					BIT(SPI_CK_IDLE_EDGE_bp)


#define SPI_CLK_GATE_REG                    SPI2_REG(0xE8)
#define SPI_CLK_EN_bp						0
#define SPI_CLK_EN_bm						BIT(SPI_CLK_EN_bp)
#define SPI_MST_CLK_ACTIVE_bp				1
#define SPI_MST_CLK_ACTIVE_bm				BIT(SPI_MST_CLK_ACTIVE_bp)
#define SPI_MST_CLK_SEL_bp					2
#define SPI_MST_CLK_SEL_bm					BIT(SPI_MST_CLK_SEL_bp)


#define SPI_DOUT_MODE_REG                   SPI2_REG(0x2C)

#define SPI_CLOCK_REG                       SPI2_REG(0x0C)

#define SPI_CTRL_REG                        SPI2_REG(0x08)
#define SPI_RD_BIT_ORDER_bp					25
#define SPI_RD_BIT_ORDER_bm					BIT(SPI_RD_BIT_ORDER_bp)
#define SPI_WR_BIT_ORDER_bp					26
#define SPI_WR_BIT_ORDER_bm					BIT(SPI_WR_BIT_ORDER_bp)


#define SPI_DMA_INT_RAW_REG					SPI2_REG(0x3C)
#define SPI_TRANS_DONE_INT_RAW_bp			12
#define SPI_TRANS_DONE_INT_RAW_bm			BIT(SPI_TRANS_DONE_INT_RAW_bp)

#define SPI_DMA_INT_CLR_REG					SPI2_REG(0x38)
#define SPI_TRANS_DONE_INT_CLR_bp			12
#define SPI_TRANS_DONE_INT_CLR_bm			BIT(SPI_TRANS_DONE_INT_CLR_bp)

/* versions register */
#define SPI_DATE_REG                        SPI2_REG(0xF0)


#define SYSREG_BASE                         0x600C0000 
#define SYSREG_REG(r)                       (SYSREG_BASE + r)

#define SYSTEM_PERIP_CLK_EN0_REG            SYSREG_REG(0x10)
#define SYSTEM_SPI2_CLK_EN_bp				6
#define SYSTEM_SPI2_CLK_EN_bm				BIT(SYSTEM_SPI2_CLK_EN_bp)

#define SYSTEM_PERIP_RST_EN0_REG            SYSREG_REG(0x18)
#define SYSTEM_SPI2_RST_bp					6
#define SYSTEM_SPI2_RST_bm					BIT(SYSTEM_SPI2_RST_bp)
/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

bool bBusIsStart;

typedef enum dataMode_e
{
	SPI_DATA_MODE_0,
	SPI_DATA_MODE_1,
	SPI_DATA_MODE_2,
	SPI_DATA_MODE_3
}dataMode_t;

typedef enum bitOrder_e
{
	SPI_BIT_ORDER_LSB,
	SPI_BIT_ORDER_MSB
}bitOrder_t;

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/

void spi_init_bus();

void spi_start_bus( uint32_t _clockDiv, const dataMode_t _dataMode, const bitOrder_t _bitOrder );

uint8_t spi_config_pins( const gpio_t _mosi, const gpio_t _miso, const gpio_t _clk, const gpio_t _ss, uint8_t _func );

void spi2_enable();

uint8_t spi_write_buffer( uint32_t *_data, uint8_t _length );

uint8_t spi_get_buffer( uint8_t _index );

void spi_reset_buffer();

void spi2_disable();

uint8_t spi_set_data_mode( const dataMode_t _dataMode );

uint8_t spi_set_bit_order( const bitOrder_t _bitOrder );

void spi_set_clock_div( uint32_t _clockDiv );

void spi_write_byte( uint8_t _data );

void spi_write_bytes( uint8_t *_data, uint8_t _length );

uint8_t spi_transfer_byte( uint8_t _data, uint8_t _length );

void spi_transfer_bytes( uint8_t *_data, uint8_t *_out, uint8_t _length );

void spi_write_word( uint16_t _data );

void spi_write_words( uint16_t * _data, uint8_t _length );

void spi_write_dword( uint32_t _data );

void spi_poll_trans_done_int();

/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SPI_H__


