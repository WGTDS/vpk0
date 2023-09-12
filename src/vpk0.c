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
    HALKEN vpk0 Tool ( Indefinitely Incomplete, but Functional ) v0.75.5

    Author  : White Guy That Don't Smile
    Date    : 2023/09/12, Tuesday, September 12th; 0715 HOURS
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
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>



#define VPK 0x76706B30



typedef  int8_t  i8;
typedef uint8_t  u8;
typedef  int16_t i16;
typedef uint16_t u16;
typedef  int32_t i32;
typedef uint32_t u32;



#define BAD_ARGS        1
#define FILE_SIZE_ERROR 2
#define RAM_UNAVAILABLE 3
#define FILE_READ_ERROR 4

static int display_error(const u32 errcode, const void *data)
{
    switch (errcode) {
        case BAD_ARGS:
            printf("??? %s\n", (char *)data);
            break;
        case FILE_SIZE_ERROR:
            printf("FILE SIZE ERROR! %d\n", *(signed *)data);
            break;
        case RAM_UNAVAILABLE:
            printf("RAM UNAVAILABLE!\n");
            break;
        case FILE_READ_ERROR:
            printf("FILE READ ERROR!\n");
            break;
        default:
            printf("\n##  HALKEN vpk0 Tool v0.75.5 [2023/09/12]  ##\n"
                   "\nUsage:  vpk0 [mode] [infile]\n"
                   "\nModes:\n  s  : Encode using Single Sampling\n"
                   "  m  : Encode using Multi  Sampling\n"
                   "  d  : Decode\n\n");
            break;
    }

    return EXIT_FAILURE;
}



/*---------------------------------------------------------------------------

                          String Inversion Section

---------------------------------------------------------------------------*/



static void strinv(void *p, size_t n)
{
    size_t rem = n >> 1;
    char *lo = (char *)p;
    char *hi = &lo[n - 1];
    char tmp;

    while (rem--) {
        tmp = *lo;
        *lo++ = *hi;
        *hi-- = tmp;
    }

    return;
}



/*---------------------------------------------------------------------------

                            Pseudo-Vector Section

---------------------------------------------------------------------------*/



#define NULL_VEC_INFO 0
#define NULL_VEC_DATA 1

typedef struct vec_s {
    size_t sz, /* volumetric size of the 'vector' */
           wd, /* width of the data type for the 'vector' */
           ct; /* count of elements stored in the 'vector' */
    void *org, /* origin pointer */
         *cur, /* current pointer address from the origin */
         *nxt; /* target pointer address relative to the volume */
} vec_t;

static const char *vsubject[2] =
{
    "information",
    "data"
};

static void verror(int status)
{
    printf("/// ERROR ///\n>>> Vector ");
    printf("%s", vsubject[status]);
    printf(" is NULL!\n");
    exit(1);
}

static vec_t *vfree(vec_t *vec)
{
    free(vec->org);
    free(vec);
    return NULL;
}

static vec_t *valloc(const size_t size)
{
    vec_t *vec = (vec_t *)calloc(1, sizeof(vec_t));

    if (vec == NULL) {
        verror(NULL_VEC_INFO);
    }
    else {
        vec->sz = 0x10000 * size;
        vec->wd = size;
        vec->org = malloc(vec->sz);

        if (vec->org == NULL) {
            verror(NULL_VEC_DATA);
        }
        else {
            vec->cur = vec->org;
            vec->nxt = (void *)((size_t)vec->cur + vec->sz);
        }
    }

    return vec;
}

static vec_t *vrealloc(vec_t *vec)
{
    if (vec == NULL) {
        verror(NULL_VEC_INFO);
    }
    else {
        char *p = NULL;

        vec->sz += (0x10000 * vec->wd);
        vec->org = realloc(vec->org, vec->sz);
        p = (char *)vec->org;

        if (vec->org == NULL) {
            verror(NULL_VEC_DATA);
        }
        else {
            vec->cur = (void *)&p[vec->ct * vec->wd];
            vec->nxt = (void *)&p[vec->sz];
        }
    }

    return vec;
}

static void vappend(vec_t *vec, const void *data)
{
    if (vec == NULL) {
        verror(NULL_VEC_INFO);
    }
    else {
        if (vec->cur < vec->nxt) {
            char *s = (char *)vec->cur;

            memcpy(s, data, vec->wd);
            vec->cur = (void *)&s[vec->wd];
            vec->ct++;
        }
        else {
            vec = vrealloc(vec);
            vappend(vec, data);
        }
    }

    return;
}



/*---------------------------------------------------------------------------

                                vpk0 Section

---------------------------------------------------------------------------*/



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

        [NUD] : SimCity 64
        [JAP][0048 / 0048] : Success

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
/*---------------------------------------------------------------------------
    Things that need to be done:
 >  Need to implement proper Huffman coding.
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
static u32 nbits = 16U;
static u16 frame = 0;
/*----------------------------------------------
    Regard clause #2 in the ToDo list as to,
    whether or not, this function needs to
    write 32-bit breadths at a time.
----------------------------------------------*/
static void store_bits(const u16 data, const u32 breadth, FILE *ofile)
{
    u32 i;

    for (i = 0; i < breadth; i++) {
        frame |= (((data >> (breadth - 1 - i)) & 1) << --nbits);

        if (nbits == 0) {
            u16 hword = frame;

            strinv(&hword, sizeof(u16));
            fwrite(&hword, sizeof(u16), 1, ofile);
            nbits = 16;
            frame = 0;
        }
    }

    return;
}

static u32 get_breadth(u32 data)
{
    u32 breadth;

    for (breadth = 1; data > 1; breadth++) data >>= 1;

    return breadth;
}

static void search(u8 *src, u8 *srcp, u8 *srcz, u32 *dict)
{
    const u8 *head, *tail;
    u32 ld[2];
    int nominal;
    u8 *window = ((srcp - src) < 0x10000U) ? &src[1] : &srcp[~0xFFFE];
    memset(dict, 0, (sizeof(u32) * 2));
    ld[0] = 1;
    nominal = ((srcp < srcz) << 2) | 2;
    nominal |= window <= srcp;

    while (nominal == 7) {
        nominal = 4;
        tail = srcp - ld[0];

        if (*tail == *srcp) {
            ld[1] = 1;
            head = srcp;

            while ((*++head == *++tail) && (++ld[1] < 0xFF));

            if (srcz <= &srcp[ld[1]]) ld[1] = srcz - srcp;

            if ((2 < ld[1]) && (dict[1] < ld[1])) {
                memcpy(dict, ld, (sizeof(u32) * 2));
            }
        }

        nominal |= ((++ld[0] < 0x10000U) << 1);
        nominal |= ++window <= srcp;
    }

    return;
}

typedef struct {
    u8 b; /* breadth value */
    u8 p; /* prefix code */
    u8 pb; /* prefix code breadth */
    u8 z; /* for alignment */
} prefix;

static void write_tree(const u8 *b, prefix *pfx, const size_t n, FILE *f)
{
    const u8 *bp = &b[0], *bz = &bp[n];
    u32 i = 0, j, y, z;

    while (bp < bz) {
        store_bits(0, 1, f);
        store_bits(*bp, 8, f);
        y = *&bp[-1];
        z = *&bp[0];
        j = get_breadth(i);

        while (y < z) {
            pfx[y].b = *bp;
            pfx[y].p = i;
            pfx[y].pb = j;
            ++y;
        }

        i |= 1;

        if (!(n % ((bp - b) + 1))) {
            i <<= 1;
        }

        ++bp;
    }

    while (bp-- > b) {
        store_bits(1, 1, f);
    }

    return;
}

static void encode(u8 *src, u8 *srcz, FILE *ofile)
{
    u8 *srcp, mask, bitflags;
    u32 dict[4];
    vec_t *bytes, *dicts, *flags;

    bitflags = 0x00U;
    bytes = valloc(1);
    dicts = valloc(2);
    flags = valloc(1);
    mask = 0x80U;
    srcp = src;

#define SHIFT_MASK \
    if ((mask >>= 1) == 0) { \
        mask = 0x80U; \
        vappend(flags, &bitflags); \
        bitflags = 0x00U; \
    }

    while (srcp < srcz) {
        search(src, srcp, srcz, &dict[0]);

        if (dict[1] < 3U) {
            vappend(bytes, srcp);
            srcp = &srcp[1];
        }
        else {
            search(src, &srcp[1], srcz, &dict[2]);

            if ((dict[1] + 1U) <= dict[3]) {
                vappend(bytes, srcp);
                srcp = &srcp[1];
                SHIFT_MASK;
                /*---------------------------------------------
                Testing against these conditions seems to fall
                in line with the method needed to reproduce the
                LZ data siphoned from preexisting "vpk0" data.
                ---------------------------------------------*/
                if (((srcp - src) & 1) | \
                    ((dict[0] + 2U) != dict[2]) | \
                    ((dict[3] - dict[1]) <= 3)) {
                    continue;
                }
                else {
                    dict[0] = dict[2];
                    dict[1] = dict[3];
                }
            }

            bitflags |= mask;
            vappend(dicts, &dict[0]);
            vappend(bytes, &dict[1]);
            srcp = &srcp[dict[1]];
        }

        SHIFT_MASK;
    }

    if (mask != 0x80U) {
        vappend(flags, &bitflags);
    }

    do {
        /* initial elements are for aligned indexing in write_tree() */
        u8 vph[] = {0, 1, 3, 10, 16}, vpb[] = {1, 2, 4, 8};
        prefix xo[17], xl[9];
        u8 *fp = (u8 *)flags->org, *bp = (u8 *)bytes->org;
        u16 *dp = (u16 *)dicts->org;
        size_t size = (flags->ct + (dicts->ct << 1) + (bytes->ct));
        u32 n = 0;

        store_bits((u16)(VPK >> 16), 16, ofile);
        store_bits((u16)VPK, 16, ofile);
        store_bits((u16)((srcz - src) >> 16), 16, ofile);
        store_bits((u16)(srcz - src), 16, ofile);
        store_bits(*srcz, 8, ofile);
        write_tree(&vph[1], &xo[1], sizeof(vph) - 1, ofile);
        write_tree(&vpb[1], &xl[1], sizeof(vpb) - 1, ofile);

        while (size) {
            if (n != 0) {
                u16 op = (bitflags & 0x80) == 0;

                store_bits(!op, 1, ofile);
                bitflags <<= 1;
                --n;

                if (op) {
                    store_bits(*bp, 8, ofile);
                    ++bp;
                    --size;
                }
                else {
                    u32 breadth;

                    if (*srcz == 0x00U) {
                        breadth = get_breadth(*dp);
                        store_bits(xo[breadth].p, xo[breadth].pb, ofile);
                        store_bits(*dp, xo[breadth].b, ofile);
                    }
                    else {
                        u32 div = (*dp + 8) >> 2, and = *dp & 3;

                        if (and) {
                            --and;
                            breadth = get_breadth(and);
                            store_bits(xo[breadth].p, xo[breadth].pb, ofile);
                            store_bits(and, xo[breadth].b, ofile);
                        }

                        breadth = get_breadth(div);
                        store_bits(xo[breadth].p, xo[breadth].pb, ofile);
                        store_bits(div, xo[breadth].b, ofile);
                    }

                    ++dp;
                    breadth = get_breadth(*bp);
                    store_bits(xl[breadth].p, xl[breadth].pb, ofile);
                    store_bits(*bp, xl[breadth].b, ofile);
                    ++bp;
                    size -= 3;
                }
            }
            else {
                bitflags = *fp++;
                n = 8;
                --size;
            }
        }

        if (nbits != 16) {
            store_bits(0, nbits, ofile);
        }
    } while (0);

    fflush(ofile);
    bytes = vfree(bytes);
    dicts = vfree(dicts);
    flags = vfree(flags);
    return;
}

static void load_bits(u16 *stream, u32 *retval)
{
    u16 t = *stream;

    strinv(&t, sizeof(u16));
    *retval = *retval << 0x10 | (u32)t;
    return;
}

void reconstruct_tree(i32 *nbits, u32 *codes, u16 **stream,
                      i32 ***huffman_tree, i32 ***data_tree)
{
    u16 *data;
    i32 **tree;
    i32 temp_nbits;
    i32 index;
    i32 is_node;

    data = *stream;
    tree = *huffman_tree;
    index = 0;

    while (1) {
        temp_nbits = *nbits;

        if (*nbits < 1) {
            load_bits(data++, &*codes);
            temp_nbits = *nbits + 0x10;
        }

        *nbits = temp_nbits + -1;
        is_node = *codes << ((0x1FU - *nbits) & 0x1F);
        is_node = is_node < 0;

        if ((is_node != 0) && (index < 2)) {
            *stream = data;
            *huffman_tree = tree;
            return;
        }

        tree[0] = NULL;
        tree[1] = NULL;
        tree[2] = NULL;

        if (is_node != 0) {
            *(i32 ***)&tree[0] = data_tree[index    ];
            *(i32 ***)&tree[1] = data_tree[index + 1];
            data_tree[index] = tree;
            index--;
        }
        else {
            if (*nbits < 8) {
                load_bits(data++, &*codes);
                *nbits = temp_nbits + 0xF;
            }

            *nbits -= 8;
            tree[2] = (i32 *)((*codes << ((0x18U - *nbits) & 0x1F)) >> 0x18);
            data_tree[index + 2] = tree;
            index++;
        }

        tree += 3;
    }
}

void traverse_tree(i32 *bits, u32 *codes, u16 **stream,
                   i32 ***huffman_tree, i32 *breadth)
{
    i32 nbits;
    u16 *data;
    i32 **tree;
    i32 branch;

    nbits = *bits;
    data = *stream;
    tree = *huffman_tree;

    if (tree[0] != (i32 *)0) {
        do {
            if (nbits < 1) {
                load_bits(data++, &*codes);
                nbits += 0xF;
            }
            else {
                nbits--;
            }

            branch = (i32)(*codes << ((0x1FU - nbits) & 0x1F));
            /* true = right = 1, false = left = 0 */
            branch = branch < 0;
            tree = (i32 **)tree[branch];
            *breadth = (i32)tree[0];
        }
        while (*breadth != 0);
    }

    *bits = nbits;
    *stream = data;
    *huffman_tree = tree;
    *breadth = (i32)tree[2];
    return;
}

static void decode(u8 *src, u8 *srcz, FILE *ofile)
{
    u8 *dst, *dstp, *dstz;
    i32 nbits, disp, breadth,
        **tree, **tree_lengths[24], **tree_offsets[22],
        *tree_buf[195];
    u32 codes, length, sample_depth;
    u16 *stream;

#define QUERY_NBITS \
    if (nbits < breadth) { \
        load_bits(stream++, &codes); \
        nbits += 0x10; \
    }

#define DIFF \
    (0x20 - breadth)

#define RECONSTRUCT_VALUE \
    ((codes << ((DIFF - nbits) & 0x1FU)) >> (DIFF & 0x1F))

    nbits = 8;
    length = *(u32 *)&src[0x04];
    strinv(&length, 4);

    if ((dst = (u8 *)calloc(length, sizeof(u8))) == NULL) {
        display_error(RAM_UNAVAILABLE, NULL);
        goto end;
    }

    dstp = dst;
    dstz = &dstp[length];
    length <<= 0x10;
    codes = *(u16 *)&src[0x08];
    strinv(&codes, 2);
    sample_depth = codes >> 8;
    codes |= length;
    stream = (u16 *)&src[0x0A];
    tree = tree_buf;
    /* Build the Offsets tree */
    tree_offsets[2] = NULL;
    reconstruct_tree(&nbits, &codes, &stream, &tree, tree_offsets);
    /* Build the Lengths tree */
    tree_lengths[2] = NULL;
    reconstruct_tree(&nbits, &codes, &stream, &tree, tree_lengths);

    /*-------------------------
    Start main decoding routine
    -------------------------*/
    if (dstp < dstz) {
        do {
            if (nbits < 1) {
                load_bits(stream++, &codes);
                nbits += 0xF;
            }
            else {
                nbits--;
            }
            /*----------------------------------------------
            Most significant bit determines operation.
            True? Parse Offset and Length Trees.
            ----------------------------------------------*/
            if ((i32)(codes << ((0x1FU - nbits) & 0x1F)) < 0) {
                tree = tree_offsets[2];
                breadth = (i32)tree[0];

                if (sample_depth == 0) { /* single sample */
                    traverse_tree(&nbits, &codes, &stream, &tree, &breadth);
                    QUERY_NBITS;
                    nbits -= breadth;
                    disp = -RECONSTRUCT_VALUE;
                }
                else { /* multi sample */
                    disp = 0;
                    traverse_tree(&nbits, &codes, &stream, &tree, &breadth);
                    QUERY_NBITS;
                    nbits -= breadth;
                    length = RECONSTRUCT_VALUE;

                    if ((i32)length < 3) {
                        disp = length + 1;
                        tree = tree_offsets[2];
                        traverse_tree(&nbits, &codes, &stream, &tree, &breadth);
                        QUERY_NBITS;
                        nbits -= breadth;
                        length = RECONSTRUCT_VALUE;
                    }

                    disp = (length * -4 - disp) + 8;
                }

                tree = tree_lengths[2];
                /* breadth = (i32)tree[2]; */
                /* assignment here for the above statement is redundant */
                /* as it occurs, regardless, in the following function. */
                traverse_tree(&nbits, &codes, &stream, &tree, &breadth);
                QUERY_NBITS;
                nbits -= breadth;
                length = RECONSTRUCT_VALUE;

                while (length--) {
                    /* < i32 :: disp > is negative here! */
                    *dstp = *&dstp[disp];
                    ++dstp;
                }
            }
            else { /* False. Copy Byte. */
                if (nbits < 8) {
                    load_bits(stream++, &codes);
                    nbits += 8;
                }
                else {
                    nbits -= 8;
                }

                *dstp++ = (char)((codes << ((0x18U - nbits) & 0x1F)) >> 0x18);
            }
        } while ((dstp < dstz) && ((u8 *)stream < srcz));
    }

    fwrite(dst, sizeof(u8), dstz - dst, ofile);
    fflush(ofile);
    free(dst);
    dst = NULL;

end:

    return;
}

static void time_elapsed(void)
{
    struct tm time;
    unsigned msec, sec;
    char fmt[12];

    msec = clock();
    sec = msec / CLOCKS_PER_SEC;
    memset(fmt, 0, 12);
    memset(&time, 0, sizeof(time));
    time.tm_yday = sec / 86400;
    time.tm_hour = sec / 3600;
    time.tm_min = (sec / 60) % 60;
    time.tm_sec = sec % 60;
    msec %= 1000;
    strftime(fmt, 12, "%Hh:%Mm:%Ss", &time);
    printf("Time Elapsed: %u day(s), %s;%.3ums\n",
           time.tm_yday, fmt, msec);
    return;
}

static float ratio(int b, float f1, float f2)
{
    float f[2] = { f1, f2 };
    int bb = f1 < f2;

    bb = !((bb == b) || (bb == !b));
    return (((bb == 1) ? (f[0] / f[1]) : (f[1] / f[0])) * 100.0f);
}

int main(int argc, char *argv[])
{
    void (*op)(u8 *, u8 *, FILE *);
    FILE *ifile = NULL, *ofile = NULL;
    u8 *src = NULL, *srcz;
    char *s, o[240];
    ssize_t isize, osize = 0;
    int character;

    if (argc != 3) {
        display_error(0, NULL);
        exit(EXIT_FAILURE);
    }

    if ((s = argv[1], s[1] || strpbrk(s, "DSMdsm") == NULL) ||
        (s = argv[2], (ifile = fopen(s, "rb")) == NULL)) {
        display_error(BAD_ARGS, (void *)s);
        exit(EXIT_FAILURE);
    }

    strcpy(o, argv[2]);

    if (toupper(*argv[1]) == 'D') {
        char *a = o, *z = &a[strlen(o)];

        a = &a[2];

        do {
            if (*z == '.') {
                *z = '\0';
                break;
            }
            else if ((*z == '\\') || (*z == '/')) {
                strcat(o, ".bin");
                break;
            }
            else if (z <= a) {
                strcat(o, "_decoded_.bin");
                break;
            }
            else {
                z = &z[-1];
            }
        } while (1);
    }
    else {
        strcat(o, ".vpk0");
    }

    if ((ofile = fopen(o, "wb")) == NULL) {
        display_error(BAD_ARGS, (void *)o);
        exit(EXIT_FAILURE);
    }

    fseek(ifile, 0L, SEEK_END);
    isize = (ssize_t)ftell(ifile);
    fseek(ifile, 0L, SEEK_SET);

    if ((isize <= 0) || (isize >= 0x3FFFFFFF)) {
        display_error(FILE_SIZE_ERROR, (void *)&isize);
        goto nil;
    }

/*  Note the "(isize + 1)". */
    if ((src = (u8 *)malloc((sizeof(u8) * (isize + 1)))) == NULL) {
        display_error(RAM_UNAVAILABLE, NULL);
        goto nil;
    }

    if (fread(src, sizeof(u8), isize, ifile) != (size_t)isize) {
        display_error(FILE_READ_ERROR, NULL);
        goto nil;
    }

    character = toupper(*argv[1]);
    op = (character > 'D') ? encode : decode;
    srcz = &src[isize];
    *srcz = (character == 'M');
    op(src, srcz, ofile);
    osize = (ssize_t)ftell(ofile);

nil:

    fclose(ofile);
    fclose(ifile);

    if (src != NULL) {
        free(src);
        src = NULL;
    }

    if (osize <= 0) {
        remove(o);
    }

    if ((isize > 0) && (osize > 0)) {
        printf(">>> IN: %u , OUT: %u , RATIO: %3.2f%%\n",
               (unsigned)isize, (unsigned)osize,
               ratio(toupper(*argv[1]) == 'E', isize, osize));
    }

    time_elapsed();
    exit(EXIT_SUCCESS);
}
