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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "vpk0.h"



#define BAD_ARGS        1
#define FILE_SIZE_ERROR 2
#define RAM_UNAVAILABLE 3
#define FILE_READ_ERROR 4
#define BAD_FILE_MAGIC  5



static int _display_error( const u32 errcode, const void *data )
{
  switch ( errcode )
  {
    case BAD_ARGS:
      printf( "??? %s\n", (char *)data );
      break;
    case FILE_SIZE_ERROR:
      printf( "FILE SIZE ERROR! %d\n", *(signed *)data );
      break;
    case RAM_UNAVAILABLE:
      printf( "RAM UNAVAILABLE!\n" );
      break;
    case FILE_READ_ERROR:
      printf( "FILE READ ERROR!\n" );
      break;
    case BAD_FILE_MAGIC:
      printf( "BAD FILE MAGIC! [0x%X]\n", *(unsigned *)data );
      break;
    default:
      printf( "\n##  HALKEN vpk0 Tool v0.75 [2022/04/25]  ##\n"
              "\nUsage:  vpk0 [mode] [infile]\n"
              "\nModes:\n  s  : Encode using Single Sampling\n"
              "  m  : Encode using Multi  Sampling\n"
              "  d  : Decode\n\n" );
      break;
  }

  return 1;
}




int main( int argc, char *argv[] )
{
  FILE *ifile = (FILE *)0;
  FILE *ofile = (FILE *)0;
  u8   *src = (u8 *)0;
  u8   *dst = (u8 *)0;
  char *s;
  char  o[240];
  u32   isize;
  u32   osize;
  u32   magic;
  u32   lapse;
  u32   sample_depth;
  int   character;
  int   slen;
  float rat;

  if ( argc != 3 ) exit( _display_error( 0, (void *)0 ) );

  if (    (s = argv[1], s[1] || strpbrk( s, "DSMdsm" ) == (char *)0)
       || (s = argv[2], (ifile =  fopen( s, "rb"  )) == (FILE *)0) )
  {
    exit( _display_error( BAD_ARGS, (void *)s ) );
  }

  strcpy( o, argv[2] );

  if ( toupper( *argv[1] ) == 'D' )
  {
    slen = (int)strlen( o );

    do
    {
      if ( o[slen] == '.' )
      {
        o[slen] = '\0';
        break;
      }
      else
      {
        if ( (o[slen] == '\\') || (o[slen] == '/') )
        {
          strcat( o, ".bin" );
          break;
        }
        else
        {
          if ( slen <= 2 )
          {
            strcat( o, "_decoded_.bin" );
            break;
          }
          else
          {
            slen--;
          }
        }
      }
    }
    while ( 1 );
  }
  else strcat( o, ".vpk0" );

  if ( (ofile = fopen( o, "wb" )) == (FILE *)0 )
  {
    exit( _display_error( BAD_ARGS, (void *)o ) );
  }

  fseek( ifile, 0L, SEEK_END );
  isize = (u32)ftell( ifile );
  fseek( ifile, 0L, SEEK_SET );

  if ( (isize == (u32)EOF) || (isize == 0) || (isize >= 0x3FFFFFFF) )
  {
    _display_error( FILE_SIZE_ERROR, (void *)&isize );
    goto nil;
  }

  if ( (src = (u8 *)calloc( isize, sizeof(u8) )) == (u8 *)0 )
  {
    _display_error( RAM_UNAVAILABLE, (void *)0 );
    goto nil;
  }

  if ( (u32)fread( src, sizeof(u8), isize, ifile ) != isize )
  {
    _display_error( FILE_READ_ERROR, (void *)0 );
    goto nil;
  }

  sample_depth = 0;
  character = toupper( *argv[1] );
  sample_depth |= (character == 'M');

  if ( character > 'D' )
  {
    encode( src, &dst, isize, &osize, sample_depth );

    if ( dst == (u8 *)0 ) goto nil;
  }
  else
  {
    magic = _swap32( *(u32 *)src );

    if ( magic != VPK )
    {
      _display_error( BAD_FILE_MAGIC, (void *)&magic );
      goto nil;
    }

    osize = _swap32( *(u32 *)(src + 4) );

    if ( (dst = (u8 *)calloc( osize, sizeof(u8) )) == (u8 *)0 )
    {
      _display_error( RAM_UNAVAILABLE, (void *)0 );
      goto nil;
    }

    decode( src, dst, osize );
  }

  fwrite( dst, sizeof(u8), osize, ofile );
  fflush( ofile );

nil:

  fclose( ofile );
  fclose( ifile );

  if ( dst != (u8 *)0 )
  {
    free( dst );
    dst = (u8 *)0;
  }
  else
  {
    remove( o );
    osize = 0;
  }

  if ( src != (u8 *)0 )
  {
    free( src );
    src = (u8 *)0;
  }

  if ( osize == 0 ) rat = 0.00;
  else
  {
    rat  = ( isize < osize ) ? ((float)isize / (float)osize)
                             : ((float)osize / (float)isize);
    rat *= 100.0;
  }

  printf( ">>> IN: %u , OUT: %u , RATIO: %3.2f%%\n",
          (unsigned)isize, (unsigned)osize, rat );
  lapse = (clock() / CLOCKS_PER_SEC);

  if ( lapse > 3599 )
  {
    printf( ">>> %u hour(s) , %u minute(s) , %u second(s) elapsed.\n",
            (u32)(lapse / 3600),
            (u32)((lapse / 60) % 60),
            (u32)(lapse % 60) );
  }
  else
  {
    if ( lapse > 59 )
    {
      printf( ">>> %u minute(s) , %u second(s) elapsed.\n",
              (u32)(lapse / 60), (u32)(lapse % 60) );
    }
    else
    {
      printf( ">>> %u second(s) elapsed.\n", lapse );
    }
  }

  exit( EXIT_SUCCESS );
}
