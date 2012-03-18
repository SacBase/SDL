#ifndef SAC_SDL2_FONT_STRUCT_H
#define SAC_SDL2_FONT_STRUCT_H

struct fontchar {
    unsigned short   width;
    short            ascent;
    short            descent;
    short            lbearing;
    short            rbearing;
    unsigned short   numbits;
    unsigned int     firstbit;
};

struct font {
    short            ascent;
    short            descent;
    unsigned short   first;
    unsigned short   last;
    struct fontchar* chars;
};

typedef unsigned int fontbits_t;

static inline unsigned getbit(fontbits_t* fontbits, unsigned bit)
{
    const unsigned word = bit / (8 * sizeof(fontbits_t));
    const unsigned flag = bit % (8 * sizeof(fontbits_t));
    return fontbits[word] & (1U << flag);
}
static inline unsigned setbit(fontbits_t* fontbits, unsigned bit)
{
    const unsigned word = bit / (8 * sizeof(fontbits_t));
    const unsigned flag = bit % (8 * sizeof(fontbits_t));
    return fontbits[word] |= (1U << flag);
}
static inline unsigned clrbit(fontbits_t* fontbits, unsigned bit)
{
    const unsigned word = bit / (8 * sizeof(fontbits_t));
    const unsigned flag = bit % (8 * sizeof(fontbits_t));
    return fontbits[word] &= ~(1U << flag);
}

#endif
