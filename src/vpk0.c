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
    HALKEN vpk0 Tool ( Indefinitely Incomplete, but Functional ) v0.75.2

    Author  : White Guy That Don't Smile
    Date    : 2022/06/30, Thursday, June 30th; 1323 HOURS
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



#define VPK 0x76706B30



typedef   signed       char i8;
typedef unsigned       char u8;
typedef   signed short int  i16;
typedef unsigned short int  u16;
#ifdef __x86_64__
typedef   signed       int  i32;
typedef unsigned       int  u32;
#else
typedef   signed long  int  i32;
typedef unsigned long  int  u32;
#endif



const char *errmsg = "RAM Unavailable!\n";



/*#########################################################################*
 *
 *      Utility Functions
 *
 *#########################################################################*/



static u16 _swap16( u16 data )
{
  return (u16)(((data & 0x00FF) << 8) |
               ((data & 0xFF00) >> 8));
}

static u32 _swap32( u32 data )
{
  return (u32)(((u32)_swap16((u16)(data & 0x0000FFFF)) << 16) |
               ((u32)_swap16((u16)((data & 0xFFFF0000) >> 16))));
}



typedef struct vec_s
{
  size_t sz;  /* size of the vector */
  size_t wd;  /* width of the data type */
  size_t ct;  /* count of elements */
  void *cur;  /* current pointer address */
  void *nxt;  /* next destination pointer address */
}
vec_t;



/*==============================================================
    Vector Allocate
==============================================================*/
static vec_t *valloc( size_t size )
{
  vec_t *vec;

  if ( (vec = calloc( 1, sizeof( vec_t ) )) == (vec_t *)0 ) goto fail;
  else
  {
    vec->sz  = 0x8000 << size;
    vec->wd  = size;
    vec->ct  = 0;
    vec->cur = calloc( vec->sz, size );

    if ( vec->cur == (void *)0 ) goto fail;
    else
    {
      vec->nxt = (void *)((size_t)vec->cur + vec->sz);
      return vec;
    }
  }

fail:

  printf( "Error allocating vector of type [%u]!\n", (unsigned)size );
  return (vec_t *)0;
}



/*==============================================================
    Vector Set Next Destination
==============================================================*/
static void _vsetnxt( vec_t *vec )
{
  vec->cur = (void *)((size_t)vec->cur + (vec->ct * vec->wd));
  return;
}



/*==============================================================
    Vector Set Beginning
==============================================================*/
static void vsetbeg( vec_t *vec )
{
  vec->cur = (void *)((size_t)vec->nxt - vec->sz);
  return;
}



/*==============================================================
    Vector Free
==============================================================*/
static size_t vfree( vec_t *vec )
{
  size_t t = 0;

  if ( vec != (vec_t *)0 )
  {
    t ^= !((vec->cur == (void *)0) || (vec->sz == 0));
    vsetbeg( vec );
    t ^= (!(((size_t)vec->nxt - vec->sz) != (size_t)vec->cur) << 1);

    if ( t != 3 )
    {
      printf( "Error for vector of type [%u].\n", (unsigned)vec->wd );

      if ( t == 1 ) printf( "Vector is unallocated!\n" );
      else printf( "Vector is misaligned!\n" );
    }
    else
    {
      free( vec->cur );
      vec->cur = (void *)0;
      vec->nxt = (void *)0;
      vec->sz  = 0;
      vec->wd  = 0;
      vec->ct  = 0;
    }

    free( vec );
    vec = (vec_t *)0;
    t = !t;
  }

  return t;
}



/*==============================================================
    Vector Reallocate
==============================================================*/
static vec_t *vrealloc( vec_t *vec )
{
  size_t t = 0;

  t |= (vec != (vec_t *)0);
  t |= (((vec->cur != (void *)0) && (vec->sz != 0)) << 1);
  vsetbeg( vec );
  t |= ((((size_t)vec->nxt - vec->sz) == (size_t)vec->cur) << 2);

  if ( t == 7 )
  {
    vec->sz += (0x8000 << vec->wd);
    vec->cur = realloc( vec->cur, vec->sz );

    if ( vec->cur == (void *)0 ) goto fail;
    else
    {
      vec->nxt = (void *)((size_t)vec->cur + vec->sz);
      _vsetnxt( vec );
      return vec;
    }
  }

fail:

  printf( "Unable to reallocate vector!\n" );
  vfree( vec );
  return (vec_t *)0;
}



/*==============================================================
    Vector Append
==============================================================*/
static vec_t *vappend( vec_t *vec, size_t data )
{
  if ( vec->cur != vec->nxt )
  {
    switch ( vec->wd )
    {
      case  1: *(u8  *)vec->cur = data;  break;
      case  2: *(u16 *)vec->cur = data;  break;
      case  4: *(u32 *)vec->cur = data;  break;
      default: printf("Unsupported!\n"); break;
    }

    vec->ct++;
    vec->cur = (void *)((size_t)vec->cur + vec->wd);
  }
  else
  {
    vec = vrealloc( vec );
    vec = vappend( vec, data );
  }

  return vec;
}



/*==============================================================
    Vector Set Integer
==============================================================*/
static void vsetint( vec_t *vec, size_t offset, size_t data )
{
  size_t o = offset * vec->wd;
  size_t p = (size_t)vec->nxt - vec->sz;

  p += o;

  switch( vec->wd )
  {
    case 1: *(u8  *)p = data; break;
    case 2: *(u16 *)p = data; break;
    case 4: *(u32 *)p = data; break;
  }

  return;
}



/*#########################################################################*
 *
 *      Encode Function
 *
 *#########################################################################*/



/*---------------------------------------------------------------------------
    HALKEN LZ Routine v1.00

    Author  : White Guy That Don't Smile
    Date    : 2022/04/25, Monday, April 25th; 2042 HOURS
    License : UnLicense | Public Domain

    This is a Lempel-Ziv compression routine siphoned from the
    AGB SDK tool "agbcomp" - modified to comply with the parameters
    recorded from decoded sample data generated by HAL Laboratory.

    ####  Confirmed LZ Parameters  ####

    Minimum Offset : 0x0001 , Minimum Length : 0x03
    Maximum Offset : 0xFFFF , Maximum Length : 0xFF

    0 Flag : Process 8 bits, verbatim.
    1 Flag : Process Length-Displacement information.

    ####  Supplementary Hypothetical Parameters  ####

    < IF >
        the next reference address, is uneven
        | OR |
        the initial match offset, plus two,
        is not-equal to
        the lookahead match offset
        | OR |
        the lookahead match length minus the initial match length,
        is less-than or equal-to
        three
    < THEN >
        go to the end of the loop
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
    ####  LZ Audits  ####

    <  Nintendo 64  >

        [N64] : Kirby 64: The Crystal Shards
        [???? / ????] : To do....

        [N64] : Pokémon Snap
        [EUR][0003 / 0003] : Success
        [JAP][0003 / 0003] : Success
        [USA][0003 / 0003] : Success

        [N64] : Shigesato Itoi's No. 1 Bass Fishing - Definitive Edition!
        [JAP][0001 / 0001] : Success

        [N64] : Super Smash Bros.
        [EUR][0516 / 0516] : Success
        [JAP][0474 / 0474] : Success
        [USA][0498 / 0498] : Success

    <  Game Boy Advance  >

        [AGB] : e-Reader ROM
        [JAP][0003 / 0003] : Success
        [JAP][0004 / 0004] : Success [e-Reader+]
        [USA][0003 / 0003] : Success

        [AGB] : Pokémon Emerald Version
        [0001 / 0001] : Success

        [AGB] : Pokémon FireRed Version
        [0001 / 0001] : Success

        [AGB] : Pokémon LeafGreen Version
        [0001 / 0001] : Success

        [AGB] : Super Mario Advance 4 - Super Mario Bros. 3
        [0001 / 0001] : Success
---------------------------------------------------------------------------*/
static void _search( u8 *src,
                     const u32  srcpos, const u32  srclen,
                           u32 *match_offset, u32 *match_length )
{
  u8 *head;
  u8 *tail;
  u32 offset;
  u32 length;
  u32 window;
  int nominal;

  *match_offset = 0;
  *match_length = 0;
  offset   = 1;
  window   = (srcpos < 0x10000) ? 1 : srcpos - 0xFFFF;
  nominal  = ((srcpos < srclen) << 2) | 2;
  nominal |= window <= srcpos;

  while ( nominal == 7 )
  {
    nominal = 4;
    tail    = &src[((i32)srcpos - offset)];

    if ( *tail == src[srcpos] )
    {
      length = 1;
      head = &src[srcpos];

      while ( (*++head == *++tail) && (++length < 0xFF) );

      if ( srclen <= (srcpos + length) ) length = srclen - srcpos;

      if ( (2 < length) && (*match_length < length) )
      {
        *match_offset = offset;
        *match_length = length;
      }
    }

    nominal |= ((++offset < 0x10000U) << 1);
    nominal |= ++window <= srcpos;
  }

  return;
}



static u8 *_lz( u8 *src, u8 *dst, const u32 srclen, u32 *dstlen )
{
  size_t cp;
  u32    match_offset;
  u32    match_length;
  u32    lookahead_offset;
  u32    lookahead_length;
  u32    mask;
  register u32 srcpos;
  u32    i;
  u8     cmds;
  u8     bytes[3];
  int    nominal;
  vec_t *lz = valloc( sizeof(u8) );

  if ( lz == (vec_t *)0 ) goto err;

  if ( vappend( lz, 0 ) == (vec_t *)0 ) goto err;

  srcpos = 0;
  cmds   = 0;
  mask   = 0x80U;
  cp     = 0;

#define shift_mask \
  if ( (mask >>= 1) == 0 ) \
  { \
    mask = 0x80U; \
    vsetint( lz, cp, cmds ); \
    cp = lz->ct; \
\
    if ( vappend( lz, 0 ) == (vec_t *)0 ) goto err;\
\
    cmds = 0; \
  }

  while ( srcpos < srclen )
  {
    _search( src, srcpos, srclen, &match_offset, &match_length );

    if ( match_length < 3U )
    {
      if ( vappend( lz, src[srcpos++] ) == (vec_t *)0 ) goto err;
    }
    else
    {
      _search( src, (srcpos + 1U), srclen,
               &lookahead_offset, &lookahead_length );

      if ( (match_length + 1U) <= lookahead_length )
      {
        if ( vappend( lz, src[srcpos++] ) == (vec_t *)0 ) goto err;

        shift_mask;
        nominal = 0;
        /*---------------------------------------------
        Testing against these conditions seems to fall
        in line with the method needed to reproduce the
        LZ data siphoned from preexisting "vpk0" data.
        ---------------------------------------------*/
        nominal |= (srcpos & 1);
        nominal |= (match_offset + 2U) != lookahead_offset;
        nominal |= (lookahead_length - match_length) <= 3;

        if ( nominal != 0 ) continue;
        else
        {
          match_offset = lookahead_offset;
          match_length = lookahead_length;
        }
      }

      cmds |= mask;
      bytes[0] = (u8)match_offset;
      bytes[1] = (u8)(match_offset >> 8);
      bytes[2] = (u8)match_length;
      i = 0;

      while ( i < 3U )
      {
        if ( vappend( lz, bytes[i++] ) == (vec_t *)0 ) goto err;
      }

      srcpos += match_length;
    }

    shift_mask;
  }

  if ( mask != 0x80U ) vsetint( lz, cp, cmds );
  else lz->ct--;

  *dstlen = (u32)lz->ct;

  if ( (dst = (u8 *)calloc( *dstlen, sizeof(u8) )) == (u8 *)0 ) goto err;

  vsetbeg( lz );
  memcpy( dst, lz->cur, (lz->ct * lz->wd) );
  vfree( lz );
  return dst;

err:

  printf( errmsg );

  if ( dst != (u8 *)0 ) free( dst );

  vfree( lz );
  return dst = (u8 *)0;
}
/*-------------------------------------------------------------------------*
 *      End HALKEN LZ Routine
 *-------------------------------------------------------------------------*/



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
static u32 _nbits = 16U;
static u16 _frame = 0;



/*----------------------------------------------
    Regard clause #2 in the ToDo list as to,
    whether or not, this function needs to
    write 32-bit breadths at a time.
----------------------------------------------*/
static void _store_bits( vec_t *buf, u16 data, u32 breadth )
{
  u32 i;

  for ( i = 0; i < breadth; i++ )
  {
    _frame |= (((data >> (breadth - 1 - i)) & 1) << --_nbits);

    if ( _nbits == 0 )
    {
      u16 hword = _swap16( _frame );

      if ( vappend( buf, hword ) == (vec_t *)0 )
      {
        printf( errmsg );
        exit( EXIT_FAILURE );
      }

      _nbits = 16;
      _frame = 0;
    }
  }

  return;
}



static u32 _get_breadth( u32 data )
{
  u32 breadth;

  for ( breadth = 0; data > 0; breadth++ ) data >>= 1;

  return breadth;
}



static u8 *encode( u8 *src, u8 *dst,
                   const u32 isize, u32 *osize,
                   const u32 sample_depth )
{
  i32  flags  = 0;
  u32  lz_pos = 0;
  u32  nflags = 0;
  u32  breadth;
  u32  div;
  u32  and;
  vec_t *bit_stream = valloc( sizeof(u16) );

  if ( bit_stream == (vec_t *)0 ) goto err;

  _store_bits( bit_stream, (u16)(VPK   >> 16), 16 );
  _store_bits( bit_stream, (u16)(VPK        ), 16 );
  _store_bits( bit_stream, (u16)(isize >> 16), 16 );
  _store_bits( bit_stream, (u16)(isize      ), 16 );
  _store_bits( bit_stream, (u16)sample_depth,   8 );
  dst = _lz( src, dst, isize, osize );

  if ( dst == (u8 *)0 ) goto err;
  else
  {
    u16 bit;

    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream,  1, 8 );
    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream,  3, 8 );
    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream, 10, 8 );
    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream, 16, 8 );
    _store_bits( bit_stream,  1, 1 );
    _store_bits( bit_stream,  1, 1 );
    _store_bits( bit_stream,  1, 1 );
    _store_bits( bit_stream,  1, 1 );

    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream,  2, 8 );
    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream,  4, 8 );
    _store_bits( bit_stream,  0, 1 );
    _store_bits( bit_stream,  8, 8 );
    _store_bits( bit_stream,  1, 1 );
    _store_bits( bit_stream,  1, 1 );
    _store_bits( bit_stream,  1, 1 );

    while ( lz_pos < *osize )
    {
      if ( nflags == 0 )
      {
        flags   = *(dst + lz_pos++);
        flags <<= 24;
        nflags  = 8;
      }
      else
      {
        bit = flags < 0;
        _store_bits( bit_stream, bit, 1 );

        if ( bit )
        {
          if ( !sample_depth )
          {
            breadth = _get_breadth( (u32)*(u16 *)(dst + lz_pos) );

            if ( breadth <= 1 )
            {
              _store_bits( bit_stream, (u16)0x0, 1 );
              breadth = 1;
            }
            else if ( breadth <= 3 )
            {
              _store_bits( bit_stream, (u16)0x2, 2 );
              breadth = 3;
            }
            else if ( breadth <= 10 )
            {
              _store_bits( bit_stream, (u16)0x6, 3 );
              breadth = 10;
            }
            else
            {
              _store_bits( bit_stream, (u16)0x7, 3 );
              breadth = 16;
            }

            _store_bits( bit_stream, *(u16 *)(dst + lz_pos), breadth );
          }
          else
          {
            div = ((*(u16 *)(dst + lz_pos) + 8) / 4);
            and = (*(u16 *)(dst + lz_pos) & 3);

            if ( and )
            {
              breadth = _get_breadth( (and - 1) );

              if ( breadth <= 1 )
              {
                _store_bits( bit_stream, (u16)0x0, 1 );
                breadth = 1;
              }
              else
              {
                _store_bits( bit_stream, (u16)0x2, 2 );
                breadth = 3;
              }

              _store_bits( bit_stream, (u16)(and - 1), breadth );
            }

            breadth = _get_breadth( div );

            if ( breadth <= 1 )
            {
              _store_bits( bit_stream, (u16)0x0, 1 );
              breadth = 1;
            }
            else if ( breadth <= 3 )
            {
              _store_bits( bit_stream, (u16)0x2, 2 );
              breadth = 3;
            }
            else if ( breadth <= 10 )
            {
              _store_bits( bit_stream, (u16)0x6, 3 );
              breadth = 10;
            }
            else
            {
              _store_bits( bit_stream, (u16)0x7, 3 );
              breadth = 16;
            }

            _store_bits( bit_stream, (u16)div, breadth );
          }

          lz_pos += 2;
          breadth = _get_breadth( (u32)*(dst + lz_pos) );

          if ( breadth <= 2 )
          {
            _store_bits( bit_stream, (u16)0x0, 1 );
            breadth = 2;
          }
          else if ( breadth <= 4 )
          {
            _store_bits( bit_stream, (u16)0x2, 2 );
            breadth = 4;
          }
          else
          {
            _store_bits( bit_stream, (u16)0x3, 2 );
            breadth = 8;
          }

          _store_bits( bit_stream, (u16)*(dst + lz_pos++), breadth );
        }
        else
        {
          _store_bits( bit_stream, (u16)*(dst + lz_pos++), 8 );
        }

        flags <<= 1;
        nflags--;
      }
    }

    if ( _nbits != 16 )
    {
      _store_bits( bit_stream, 0, _nbits );
    }

    *osize = bit_stream->ct << 1;

    if ( (dst = (u8 *)calloc( *osize, sizeof(u8) )) == (u8 *)0 ) goto err;

    vsetbeg( bit_stream );
    memcpy( dst, bit_stream->cur, (bit_stream->ct * bit_stream->wd) );
    vfree( bit_stream );
  }

  return dst;

err:

  printf( "Unsuccessful process!\n" );

  if ( bit_stream != (vec_t *)0 )
  {
    vfree( bit_stream );
  }

  if ( dst != (u8 *)0 )
  {
    free( dst );
    dst = (u8 *)0;
  }

  *osize = 0;

  return (u8 *)0;
}



/*#########################################################################*
 *
 *      Decode Function
 *
 *#########################################################################*/



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
static void decode( const u8 *src, u8 *dst, const u32 dstlen )
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



/*#########################################################################*
 *
 *      Main Function
 *
 *#########################################################################*/



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
      printf( "\n##  HALKEN vpk0 Tool v0.75.2 [2022/06/30]  ##\n"
              "\nUsage:  vpk0 [mode] [infile]\n"
              "\nModes:\n  s  : Encode using Single Sampling\n"
              "  m  : Encode using Multi  Sampling\n"
              "  d  : Decode\n\n" );
      break;
  }

  return EXIT_FAILURE;
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
    dst = encode( src, dst, isize, &osize, sample_depth );

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
