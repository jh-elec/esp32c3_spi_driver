
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

//#define SPI_USE_SW_CS0
#define SPI_USE_HW_CS0

#ifdef SPI_USE_SW_CS0
	#warning Software "SlaveSelect" is enabled for SPI
#endif

#ifdef SPI_USE_HW_CS0
	#warning Hardware "SlaveSelect" is enabled for SPI
#endif 



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

/* get xtal frequency in mhz */
uint32_t spi_get_xtal_clk_freq();

/* initialize the spi bus  */
void spi_init_bus();

/* spi start bus
*	_clockDiv: config the prescaler for frequency
*	_dataMode: spi data mode (0..3)
*	_bitOrder: msb or lsb first
*/
void spi_start_bus( uint32_t _clockDiv, const dataMode_t _dataMode, const bitOrder_t _bitOrder );

/* enable system clock for the bus */
void spi2_enable();

/* write data to spi transfer buffer
*	_*data: pointer to data for the 0..15 transfer buffer
*	_length: data length to write in buffer
*/
uint8_t spi_write_buffer( uint32_t *_data, uint8_t _length );

/* spi get receive buffer 
*	_index: buffer to be read (0..15)
*/
uint32_t spi_get_buffer( uint8_t _index );

/* reset the spi transfer buffers (0..15) */
void spi_reset_buffer();

/* disable clock and enable reset for spi */
void spi2_disable();

/* spi start bus
*	_dataMode: config data mode 
*/
uint8_t spi_set_data_mode( const dataMode_t _dataMode );

/* set most significant bit or least significant bit
*	_bitOrder: most significant bit or least significant bit
*/
uint8_t spi_set_bit_order( const bitOrder_t _bitOrder );

/* set clock divisor
*	_clockDiv: clock divisor 
*/
void spi_set_clock_div( uint32_t _clockDiv );

/* spi write byte 
*	_data: data to be transmit
*/
void spi_write_byte( uint8_t _data );

/* spi write bytes
*	_*data: pointer to data
*	_length: length of data
*/
void spi_write_bytes( uint8_t *_data, uint8_t _length );

/* spi write bytes and receive one byte
*	_data: data transmit
*	_length: length of data
*/
uint8_t spi_transfer_byte( uint8_t _data );

/* spi transfer bytes
*	_data*: pointer of data
*	_out*: buffer for received data
*	_length: data for transmit / receive
*/
void spi_transfer_bytes( uint8_t *_data, uint8_t *_out, uint8_t _length );

/* spi write word
*	_data: data transmit
*/
void spi_write_word( uint16_t _data );

/* spi transfer word
*	_data: data transmit
*	return: received data
*/
uint16_t spi_transfer_word( uint16_t _data );

/* spi transfer words
*	_data: data transmit
*	_length: length of data to be write
*/
void spi_write_words( uint16_t *_data, uint8_t _length );

/* spi transfer words
*	_*data: pointer to data for transmit
*	_*out: pointer to be store received data
*	_length: length of transmit / received data
*/
void spi_transfer_words( uint16_t *_data, uint16_t *_out, uint8_t _length );

/* spi write words
*	*_data: pointer to data for transmit
*	_length: length of data for transmit
*/
void spi_write_words( uint16_t * _data, uint8_t _length );

/* spi write double word
*	_data: data transmit
*/
void spi_write_dword( uint32_t _data );

/* spi transfer double word
*	_data: data transmit
*	_length: length of data for transmit
*	return: received data
*/
uint32_t spi_transfer_dword( uint32_t _data, uint8_t _length );

/* spi transfer double word
*	_data: data transmit
*	_length: length of data for transmit
*	return: received data
*/
void spi_write_dwords( uint32_t *_data, uint8_t _length );

/* spi transfer double words
*	*_data: pointer of data to write
*	*_out: pointer to store received data
*	_length: length of data for write
*/
void spi_transfer_dwords( uint32_t *_data, uint32_t *_out, uint8_t _length );

/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SPI_H__


