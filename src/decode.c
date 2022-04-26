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
#include "vpk0.h"



static void _load_bits( stream, retval )
u16 *stream;
u32 *retval;
{
  *retval = *retval << 0x10 | (u32)_swap16( *stream );
  return;
}



/*----------------------------------
RSE Suite Used: Ghidra
ROM Used: Pokémon Snap [Nintendo 64]
Function: vpk0 Data Decoder
----------------------------------*/
void decode( const u8 *src, u8 *dst, const u32 dstlen )
{
  i32   nbits;
  i32   temp_nbits;
  u32   codes;
  u16  *stream;
  i32 **tree;
  i32   count; /* affiliated with < breadth >, used with length breadths. */
  /*-----------------------------------
  < i32 :: is_node >
  Most significant bit determines node.
  -----------------------------------*/
  i32   is_node;
  u8   *dest;
  i32  *breadth; /* affiliated with < count >, used with offset breadths. */
  u32   length;
  u8   *prev;
  i32 **tree_lengths[24];
  i32 **tree_offsets[22];
  u32   sample_depth; /* 0 == single, 1 == multi; for offset breadths. */
  i32  *tree_buf[195];

  dest  = dst + dstlen;
  nbits = 8;
  codes = _swap32( *(i32 *)(src + 4) ) << 0x10 |
          (u32)_swap16( *(u16 *)(src + 8) );
  sample_depth = ((u32)_swap16( *(u16 *)(src + 8) ) << 0x10) >> 0x18;
  stream = (u16 *)(src + 10);
  /*--------------------
  Build the Offsets tree
  --------------------*/
  tree_offsets[2] = (i32 **)0x0;
  tree  = tree_buf;
  count = 0;

  while ( 1 )
  {
    temp_nbits = nbits;

    if ( nbits < 1 )
    {
      _load_bits( stream++, &codes );
      temp_nbits = nbits + 0x10;
    }

    nbits   = temp_nbits + -1;
    is_node = codes << ((0x1FU - nbits) & 0x1F);

    if ( (is_node < 0) && (count < 2) ) break;

    if ( is_node < 0 )
    {
      tree[0] = (i32 *)0x0;
      tree[1] = (i32 *)0x0;
      tree[2] = (i32 *)0x0;
      *(i32 ***)(tree    ) = tree_offsets[count    ];
      *(i32 ***)(tree + 1) = tree_offsets[count + 1];
      tree_offsets[count]  = tree;
      tree += 3;
      count--;
    }
    else
    {
      tree[0] = (i32 *)0x0;
      tree[1] = (i32 *)0x0;
      tree[2] = (i32 *)0x0;

      if ( nbits < 8 )
      {
        _load_bits( stream++, &codes );
        nbits = temp_nbits + 0xF;
      }

      nbits  -= 8;
      tree[2] = (i32 *)((codes << ((0x18U - nbits) & 0x1F)) >> 0x18);
      tree_offsets[count + 2] = tree;
      count++;
      tree += 3;
    }
  }
  /*--------------------
  Build the Lengths tree
  --------------------*/
  tree_lengths[2] = (i32 **)0x0;
  count = 0;

  while ( 1 )
  {
    temp_nbits = nbits;

    if ( nbits < 1 )
    {
      _load_bits( stream++, &codes );
      temp_nbits = nbits + 0x10;
    }

    nbits   = temp_nbits + -1;
    is_node = codes << ((0x1FU - nbits) & 0x1F);

    if ( (is_node < 0) && (count < 2) ) break;

    if ( is_node < 0 )
    {
      tree[0] = (i32 *)0x0;
      tree[1] = (i32 *)0x0;
      tree[2] = (i32 *)0x0;
      *(i32 ***)(tree    ) = tree_lengths[count    ];
      *(i32 ***)(tree + 1) = tree_lengths[count + 1];
      tree_lengths[count]  = tree;
      tree += 3;
      count--;
    }
    else
    {
      tree[0] = (i32 *)0x0;
      tree[1] = (i32 *)0x0;
      tree[2] = (i32 *)0x0;

      if ( nbits < 8 )
      {
        _load_bits( stream++, &codes );
        nbits = temp_nbits + 0xF;
      }

      nbits  -= 8;
      tree[2] = (i32 *)((codes << ((0x18U - nbits) & 0x1F)) >> 0x18);
      tree_lengths[count + 2] = tree;
      count++;
      tree += 3;
    }
  }
  /*-------------------------
  Start main decoding routine
  -------------------------*/
  if ( dst < dest )
  {
    do
    {
      if ( nbits < 1 )
      {
        _load_bits( stream++, &codes );
        nbits += 0xF;
      }
      else
      {
        nbits--;
      }
      /*----------------------------------------------
      Most significant bit determines operation.
      True? Parse Offset and Length Trees.
      ----------------------------------------------*/
      if ( (i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0 )
      {
        tree = tree_offsets[2];
        breadth = tree[0];

        if ( sample_depth == 0 ) /* single sample */
        {
          while ( breadth != (i32 *)0x0 )
          {
            if ( nbits < 1 )
            {
              _load_bits( stream++, &codes );
              nbits += 0xF;
            }
            else
            {
              nbits--;
            }

            if ( (i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0 )
            {
              tree = (i32 **)tree[1];
            }
            else
            {
              tree = (i32 **)tree[0];
            }

            breadth = tree[0];
          }

          breadth = tree[2];

          if ( nbits < (i32)breadth )
          {
            _load_bits( stream++, &codes );
            nbits += 0x10;
          }

          nbits -= (i32)breadth;
          count  = -((codes << (((0x20 - (i32)breadth) - nbits) & 0x1FU))
                     >> ((0x20U - (i32)breadth) & 0x1F));
        }
        else /* multi sample */
        {
          count = 0;

          while ( breadth != (i32 *)0x0 )
          {
            if ( nbits < 1 )
            {
              _load_bits( stream++, &codes );
              nbits += 0xF;
            }
            else
            {
              nbits--;
            }

            if ( (i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0 )
            {
              tree = (i32 **)tree[1];
            }
            else
            {
              tree = (i32 **)tree[0];
            }

            breadth = tree[0];
          }

          breadth = tree[2];

          if ( nbits < (i32)breadth )
          {
            _load_bits( stream++, &codes );
            nbits += 0x10;
          }

          nbits -= (i32)breadth;
          length = (codes << (((0x20 - (i32)breadth) - nbits) & 0x1FU))
                   >> ((0x20U - (i32)breadth) & 0x1F);

          if ( (i32)length < 3 )
          {
            count = length + 1;
            tree  = tree_offsets[2];
            breadth = tree[0];

            while ( breadth != (i32 *)0x0 )
            {
              if ( nbits < 1 )
              {
                _load_bits( stream++, &codes );
                nbits += 0xF;
              }
              else
              {
                nbits--;
              }

              if ( (i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0 )
              {
                tree = (i32 **)tree[1];
              }
              else
              {
                tree = (i32 **)tree[0];
              }

              breadth = tree[0];
            }

            breadth = tree[2];

            if ( nbits < (i32)breadth )
            {
              _load_bits( stream++, &codes );
              nbits += 0x10;
            }

            nbits -= (i32)breadth;
            length = (codes << (((0x20 - (i32)breadth) - nbits) & 0x1FU))
                     >> ((0x20U - (i32)breadth) & 0x1F);
          }

          count = (length * -4 - count) + 8;
        }

        prev  = dst + count; /* < i32 :: count > is negative here! */
        tree  = tree_lengths[2];
        count = (i32)tree[2];

        if ( tree[0] != (i32 *)0x0 )
        {
          do
          {
            if ( nbits < 1 )
            {
              _load_bits( stream++, &codes );
              nbits += 0xF;
            }
            else
            {
              nbits--;
            }

            if ( (i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0 )
            {
              tree = (i32 **)tree[1];
            }
            else
            {
              tree = (i32 **)tree[0];
            }
          }
          while ( tree[0] != (i32 *)0x0 );

          count = (i32)tree[2];
        }

        if ( nbits < count )
        {
          _load_bits( stream++, &codes );
          nbits += 0x10;
        }

        nbits -= count;
        length = (codes << (((0x20 - count) - nbits) & 0x1FU))
                 >> ((0x20U - count) & 0x1F);

        while ( *dst++ = *prev++, --length );
      }
      else /* False. Copy Byte. */
      {
        if ( nbits < 8 )
        {
          _load_bits( stream++, &codes );
          nbits += 8;
        }
        else
        {
          nbits -= 8;
        }

        *dst++ = (char)((codes << ((0x18U - nbits) & 0x1F)) >> 0x18);
      }
    }
    while ( dst < dest );
  }

  dst -= dstlen;
  return;
}
