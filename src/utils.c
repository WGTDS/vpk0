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
    HALKEN vpk0 Tool ( Work in Progress ) v0.75.1

    Author  : White Guy That Don't Smile
    Date    : 2022/05/09, Monday, May 9th; 2220 HOURS
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

u16 _swap16( u16 data )
{
  return (u16)(((data & 0x00FF) << 8) |
               ((data & 0xFF00) >> 8));
}

u32 _swap32( u32 data )
{
  return (u32)(((u32)_swap16((u16)(data & 0x0000FFFF)) << 16) |
               ((u32)_swap16((u16)((data & 0xFFFF0000) >> 16))));
}
