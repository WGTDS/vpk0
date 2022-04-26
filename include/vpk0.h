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
#ifndef LIBSLI_H_INCLUDED
#define LIBSLI_H_INCLUDED



#define VPK 0x76706B30



/*------------------------------------------------------
At the expense of RAM, adjusting this macro value
will improve execution time during encoding by reducing
the frequency of re-allocation calls with the work
buffer(s) in use, in addition to, increasing the
possibility of errors concerning integer overflows or
allocation rejection.
The aforementioned macro corresponds to the number of
bits that will be left-shifted by a hard-coded value
as part of an incremental supplement in conjunction
with work RAM buffers used for storing encoded data
representation elements.
[Safe]
  0 =   4096 (Standard)
  1 =   8192
[Risky]
  2 =  16384
  3 =  32768
[Asking for Trouble]
  4 =  65536
  5 = 131072
[Absolute Madman]
  6 = 262144
  7 = 524288
------------------------------------------------------*/
#define ENC_GREED 2



typedef   signed char  i8;
typedef unsigned char  u8;
typedef   signed short i16;
typedef unsigned short u16;
#ifdef __x86_64__
typedef   signed int   i32;
typedef unsigned int   u32;
#else
typedef   signed long  i32;
typedef unsigned long  u32;
#endif



u16 _swap16( u16 data );
u32 _swap32( u32 data );
u8 *_lz( u8 *src, u8 **dst, const u32 srclen, u32 *dstlen );
void encode( u8 *src, u8 **dst,
             const u32 srclen, u32 *dstlen,
             const u32 sample_depth );
void decode( const u8 *src, u8 *dst, const u32 decsz );



#endif
