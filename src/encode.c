/****************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 ***************************************************************************/
 /*--------------------------------------------------------------------------
    HALKEN vpk0 Tool ( Work in Progress ) v0.75

    Author  : White Guy That Don't Smile
    Date    : 2022/04/25, Monday, April 25th; 2042 HOURS
    License : UnLicense | Public Domain

    This is a data compression tool presented with the goal of
    *faithfully reverse-engineering the compression aspect of
    HAL Laboratory's enigmatic algorithm.
    It supports decompression by using the code found within
    one of their Nintendo 64 titles, Pokémon Snap.

    As of present, the compression portion is at 50% with the LZ
    routine being faithfully deduced (I hope).

    The exact Huffman routine has yet to be unraveled, therefore,
    this tool temporarily uses bit-stream LZ encoding.

    *This project yields inspiration from CaitSith2 in a number
    of ways, however, the key motive for this project is to
    replicate the format, _faithfully_ (i.e. 1:1 encoded data).
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
    Things that need to be done:
 >  Need to implement Huffman coding.
    For now, this routine borrows CaitSith2's "-level 1" for functionality.
    Once the Huffman aspect has been implemented, this functionality
    will be rescinded for conformance.
 >  Need to verify that writing 16 bits at a time is a legal operation.
    The decoding routine from the N64 loads this breadth amount at a time,
    however, this is likely invalid with hardware like the e-Reader.
 >  Need to review the 0x01 Method conditional routine.
    As of present, it is based on both the reference from CaitSith2's
    solution, as well as, the decoding routine from Pokémon Snap.
    While the code for this tool can decode the encoded 0x01 data
    it generates, CaitSith2's "nvpktool" is unable to decode the
    aforementioned data.
    It's a conflicting observation.
---------------------------------------------------------------------------*/
#include "vpk0.h"
#include <stdio.h>
#include <stdlib.h>



static u32 _nbits = 16U;
static u16 _frame = 0;



/*----------------------------------------------
    Regard clause #2 in the ToDo list as to,
    whether or not, this function needs to
    write 32-bit breadths at a time.
----------------------------------------------*/
static void _store_bits( u16 *buf, u32 *cursor,
                         u16 data, u32 breadth )
{
  u32 i;

  for ( i = 0; i < breadth; i++ )
  {
    _frame |= (((data >> (breadth - 1 - i)) & 1) << --_nbits);

    if ( _nbits == 0 )
    {
      *(buf + *cursor) = _swap16( _frame );
      (*cursor)++;
      _nbits = 16;
      _frame = 0;
    }
  }

  return;
}



void encode( u8 *src, u8 **dst,
             const u32 isize, u32 *osize,
             const u32 sample_depth )
{
  i32  flags  = 0;
  u8  *retval = (u8  *)0;
  u32  lz_pos = 0;
  u32  nflags = 0;
  u32  cursor = 0;
  u32  cursor_boundary = 0x1000U << ENC_GREED;
  u16 *bit_stream = (u16 *)calloc( cursor_boundary , sizeof(u16) );

#define extend_buffer \
  if ( cursor == cursor_boundary ) \
  { \
    cursor_boundary += (0x1000U << ENC_GREED); \
    bit_stream = realloc( bit_stream, (cursor_boundary * 2U) ); \
\
    if ( bit_stream == (u16 *)0 ) goto err; \
  }

  if ( bit_stream == (u16 *)0 ) goto err;

  _store_bits( bit_stream, &cursor, (u16)(VPK   >> 16), 16 );
  _store_bits( bit_stream, &cursor, (u16)(VPK        ), 16 );
  _store_bits( bit_stream, &cursor, (u16)(isize >> 16), 16 );
  _store_bits( bit_stream, &cursor, (u16)(isize      ), 16 );
  _store_bits( bit_stream, &cursor, (u16)sample_depth,   8 );
  retval = _lz( src, &*dst, isize, &*osize );

  if ( retval == (u8 *)0 ) goto err;
  else
  {
    u16 bit;

    _store_bits( bit_stream, &cursor,  0, 1 );
    _store_bits( bit_stream, &cursor, 16, 8 );
    _store_bits( bit_stream, &cursor,  1, 1 );
    _store_bits( bit_stream, &cursor,  0, 1 );
    _store_bits( bit_stream, &cursor,  8, 8 );
    _store_bits( bit_stream, &cursor,  1, 1 );

    while ( lz_pos < *osize )
    {
      if ( nflags == 0 )
      {
        flags   = *(*dst + lz_pos++);
        flags <<= 24;
        nflags  = 8;
      }
      else
      {
        bit = flags < 0;
        _store_bits( bit_stream, &cursor, bit, 1 );
        extend_buffer;

        if ( bit )
        {
          if ( !sample_depth )
          {
            _store_bits( bit_stream, &cursor, *(u16 *)(*dst + lz_pos), 16 );
            extend_buffer;
          }
          else
          {
            if ( (*(u16 *)(*dst + lz_pos) % 4) == 0 )
            {
              _store_bits( bit_stream,
                           &cursor,
                           (u16)((*(u16 *)(*dst + lz_pos) + 8) / 4),
                           16 );
              extend_buffer;
            }
            else
            {
              _store_bits( bit_stream,
                           &cursor,
                           (u16)((*(u16 *)(*dst + lz_pos) % 4) + -1),
                           16 );
              extend_buffer;
              _store_bits( bit_stream,
                           &cursor,
                           (u16)((*(u16 *)(*dst + lz_pos) + 8) / 4),
                           16 );
              extend_buffer;
            }
          }

          lz_pos += 2;
          _store_bits( bit_stream, &cursor, (u16)*(*dst + lz_pos++), 8 );
          extend_buffer;
        }
        else
        {
          _store_bits( bit_stream, &cursor, (u16)*(*dst + lz_pos++), 8 );
          extend_buffer;
        }

        flags <<= 1;
        nflags--;
      }
    }

    if ( _nbits != 16 )
    {
      _store_bits( bit_stream, &cursor, 0, _nbits );
      extend_buffer;
    }

    *osize = cursor << 1;
    *dst = (u8 *)bit_stream;
  }

  return;

err:

  printf( "Unsuccessful process!\n" );

  if ( retval != (u8 *)0 )
  {
    free( retval );
    retval = (u8 *)0;
  }

  if ( bit_stream != (u16 *)0 )
  {
    free( bit_stream );
    bit_stream = (u16 *)0;
  }

  if ( *dst != (u8 *)0 )
  {
    free( *dst );
    *dst = (u8 *)0;
  }

  *osize = 0;

  return;
}
