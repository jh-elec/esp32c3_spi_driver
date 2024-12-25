
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
#include "spi.h"



static inline uint16_t swap_word( uint16_t _data )
{
  return ( _data >> 8 | _data << 8 );
}

static inline uint32_t swap_dword(uint32_t _data )
{
  return (((_data & 0x000000ffU) << 24) |
          ((_data & 0x0000ff00U) <<  8) |
          ((_data & 0x00ff0000U) >>  8) |
          ((_data & 0xff000000U) >> 24));
}



void spi_init_bus() 
{
  REG_WRITE( SPI_SLAVE_REG, 0 );
  REG_WRITE( SPI_MISC_REG, 0 );
  REG_WRITE( SPI_USER_REG, 0 );
  REG_WRITE( SPI_USER1_REG, 0 );
  REG_WRITE( SPI_CTRL_REG, 0 );
  REG_WRITE( SPI_CLK_GATE_REG, 0 );
  REG_WRITE( SPI_DMA_CONF_REG, 0 );
  REG_SET_BIT( SPI_DMA_CONF_REG, SPI_RX_AFIFO_RST_bm );
  REG_SET_BIT( SPI_DMA_CONF_REG, SPI_BUF_AFIFO_RST_bm );
  REG_WRITE( SPI_CLOCK_REG, 0 );

  //REG_SET_BIT( SPI_USER_REG,  SPI_USR_CONF_NXT_bm );
}

void spi_start_bus( uint32_t _clockDiv, const dataMode_t _dataMode, const bitOrder_t _bitOrder )
{
  spi2_enable();
  spi_init_bus();

  REG_WRITE( SPI_CLK_GATE_REG, ( SPI_CLK_EN_bm | SPI_MST_CLK_ACTIVE_bm | SPI_MST_CLK_SEL_bm ) );
  REG_SET_BIT( SPI_USER_REG, ( SPI_USR_MISO_bm | SPI_USR_MOSI_bm | SPI_DOUTDIN_bm ) );

  for ( uint32_t i =  0 ; i < 16 ; i++ )
  {
    REG_WRITE( SPI_Wn_REG(i), 0 );
  }

  spi_set_data_mode( _dataMode );
  spi_set_bit_order( _bitOrder );
  spi_set_clock_div( _clockDiv );
}

void spi2_enable()
{
  REG_CLR_BIT( SYSTEM_PERIP_RST_EN0_REG, SYSTEM_SPI2_RST_bm ); 
  REG_SET_BIT( SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_SPI2_CLK_EN_bm ); 
}

uint8_t spi_write_buffer( uint32_t *_data, uint8_t _length )
{
  if ( _length > 16 )
  {
    return 1; // error
  }

  for ( uint8_t x = 0; x < _length ; x++ )
  {
    REG_WRITE( SPI_Wn_REG(x), *_data++ );
  }

  return 0; // all_ok
}

uint8_t spi_get_buffer( uint8_t _index )
{
  return REG_READ( SPI_Wn_REG(_index) );
}

void spi_reset_buffer()
{
  REG_SET_BIT( SPI_DMA_CONF_REG, ( SPI_DMA_AFIFO_RST_bm | SPI_BUF_AFIFO_RST_bm | SPI_RX_AFIFO_RST_bm ) );
}

void spi2_disable()
{
  REG_SET_BIT( SYSTEM_PERIP_RST_EN0_REG,  SYSTEM_SPI2_RST_bm); 
  REG_CLR_BIT( SYSTEM_PERIP_CLK_EN0_REG,  SYSTEM_SPI2_CLK_EN_bm); 
}

uint8_t spi_set_data_mode( const dataMode_t _dataMode )
{
    switch( _dataMode )
    {
        case SPI_DATA_MODE_0:
          REG_CLR_BIT( SPI_MISC_REG, SPI_CK_IDLE_EDGE_bm); 
          REG_SET_BIT( SPI_USER_REG, SPI_CK_OUT_EDGE_bm); 
        break;

        case SPI_DATA_MODE_1:
          REG_SET_BIT( SPI_MISC_REG, SPI_CK_IDLE_EDGE_bm); 
          REG_SET_BIT( SPI_USER_REG, SPI_CK_OUT_EDGE_bm); 
        break;

        case SPI_DATA_MODE_2:
          REG_SET_BIT( SPI_MISC_REG, SPI_CK_IDLE_EDGE_bm); 
          REG_CLR_BIT( SPI_USER_REG, SPI_CK_OUT_EDGE_bm);
        break;

        case SPI_DATA_MODE_3:
          REG_CLR_BIT( SPI_MISC_REG, SPI_CK_IDLE_EDGE_bm); 
          REG_CLR_BIT( SPI_USER_REG, SPI_CK_OUT_EDGE_bm); 
        break;
		
		default:
			return 1; // wrong data mode
		break;
    }
	
	return 0;
}

uint8_t spi_set_bit_order( const bitOrder_t _bitOrder )
{
  switch ( _bitOrder )
  {
    case SPI_BIT_ORDER_LSB: 
      REG_SET_BIT( SPI_CTRL_REG, ( SPI_WR_BIT_ORDER_bm | SPI_RD_BIT_ORDER_bm ) );
    break;

    case SPI_BIT_ORDER_MSB: 
      REG_CLR_BIT( SPI_CTRL_REG, ( SPI_WR_BIT_ORDER_bm | SPI_RD_BIT_ORDER_bm ) );
    break;
	
	default:
		  return 1; // wrong bit order
	break;
  }
  
  return 0;
}

void spi_set_clock_div( uint32_t _clockDiv )
{
  REG_WRITE( SPI_CLOCK_REG, _clockDiv );
}

void spi_write_byte( uint8_t _data )  
{
  REG_WRITE( SPI_MS_DLEN_REG, 7 );
  REG_WRITE( SPI_Wn_REG(0), _data );

  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm );
}

void spi_write_bytes( uint8_t *_data, uint8_t _length )
{
  if (_length > 64) 
  {
    _length = 64;
  }

  uint32_t words = (_length + 3) / 4;  //16 max
  uint32_t wordsBuf[16] = {0};
  uint8_t *bytesBuf = (uint8_t *)wordsBuf;

  for (uint8_t i = 0; i < _length; i++)
  {
    bytesBuf[i] = _data[i];
  }

  REG_WRITE( SPI_MS_DLEN_REG, ( 8 * _length ) -1 );

  for (uint8_t i = 0; i < words; i++)
  {
    REG_WRITE( SPI_Wn_REG( i ), wordsBuf[i]);
  }
  
  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm );  

  spi_poll_trans_done_int();
}

uint8_t spi_transfer_byte( uint8_t _data, uint8_t _length )
{
  REG_WRITE( SPI_MS_DLEN_REG, 7 );
  REG_WRITE( SPI_Wn_REG(0), _data );

  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm );

  return spi_get_buffer( 0 );
}

void spi_transfer_bytes( uint8_t *_data, uint8_t *_out, uint8_t _length )
{
  if( _length > 16 ) _length = 16;

  REG_WRITE( SPI_MS_DLEN_REG, (  8 * _length ) -1 );

  spi_write_buffer( (uint32_t*) _data, _length );

  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm );  

  for (uint8_t i = 0; i < _length ; i++)
  {
    *_out++ = spi_get_buffer( i );
  }
}

void spi_write_word( uint16_t _data )
{
  REG_WRITE( SPI_MS_DLEN_REG, 15 );
  REG_WRITE( SPI_Wn_REG(0), swap_word( _data ) );

  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm );   
}

void spi_write_words( uint16_t * _data, uint8_t _length )
{
  if( _length > 16 ) _length = 16;  

  REG_WRITE( SPI_MS_DLEN_REG,(  ( 16 * _length) -1 ) );
  
  for (uint8_t i = 0; i < _length; i++)
  {
    REG_WRITE( SPI_Wn_REG( i ),  swap_word( *_data++ ) );
  }
  
  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm ){};     
}

void spi_write_dword( uint32_t _data )
{
  REG_WRITE( SPI_MS_DLEN_REG, 32 -1 );
  REG_WRITE( SPI_Wn_REG(0), _data );

  REG_SET_BIT( SPI_CMD_REG, SPI_UPDATE_bm );

  REG_SET_BIT( SPI_CMD_REG, SPI_USR_bm );
  while ( REG_READ( SPI_CMD_REG ) & SPI_USR_bm ){};    

  spi_poll_trans_done_int();
}

void spi_poll_trans_done_int()
{
  while( ! ( REG_READ( SPI_DMA_INT_RAW_REG )  & SPI_TRANS_DONE_INT_RAW_bm ));
}

void spi_trans_done_clear_int()
{
  REG_SET_BIT( SPI_DMA_INT_CLR_REG, SPI_TRANS_DONE_INT_CLR_bm );
}