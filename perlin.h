#if defined (__cplusplus)
# pragma once
#endif /* __cplusplus */
#
#if !defined (_perlin_h_)
# define _perlin_h_ 1
#
# if !defined PERLINAPI
#  define PERLINAPI extern
# endif /* PERLINAPI */
#
# if defined (__cplusplus)

extern "C" {

# endif /* __cplusplus */

PERLINAPI double perlin2d(double, double);

PERLINAPI double perlin3d(double, double, double);

# if defined (__cplusplus)

}

# endif /* __cplusplus */
#
# if defined (PERLIN_IMPLEMENTATION)
#
#  include <stddef.h>
#  include <stdint.h>
#
#  include <math.h>
#
#  if defined (__cplusplus)

extern "C" {

#  endif /* __cplusplus */

/* */
static const uint8_t g_perm[512] = {
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
};

static inline double __perlin_floor(double a) {
    int i = (int) a;
    return ((a < i) ? i - 1 : i);
}


static inline double __perlin_fade(double t) { return (t * t * t * (t * (t * 6.0 - 15.0) + 10.0)); }


static inline double __perlin_lerp(double t, double a, double b) { return (a + t * (b - a)); } 


static inline double __perlin_grad(int hash, double x, double y, double z) {
    int h = hash & 0x0f;
    double u = h < 8 ? x : y,
           v = h < 4 ? y : h == 12 || h == 14 ? x : z;

    return (((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v));
}


static inline double __perlin(double x, double y, double z, int x_wrap, int y_wrap, int z_wrap) {
    uint32_t x_mask = (x_wrap - 1) & 0xff,
             y_mask = (y_wrap - 1) & 0xff,
             z_mask = (z_wrap - 1) & 0xff;

    int32_t x_floor = __perlin_floor(x),
            y_floor = __perlin_floor(y),
            z_floor = __perlin_floor(z);

    int32_t X = x_floor & x_mask,
            Y = y_floor & y_mask,
            Z = z_floor & z_mask;

    x -= x_floor;
    y -= y_floor;
    z -= z_floor;

    double u = __perlin_fade(x),
           v = __perlin_fade(y),
           w = __perlin_fade(z);

    uint8_t A = g_perm[X    ] + Y, AA = g_perm[A] + Z, AB = g_perm[A + 1] + Z,
            B = g_perm[X + 1] + Y, BA = g_perm[B] + Z, BB = g_perm[B + 1] + Z;

    return (__perlin_lerp(w, __perlin_lerp(v, __perlin_lerp(u, __perlin_grad(g_perm[AA    ], x      , y      , z      ),
                                                               __perlin_grad(g_perm[BA    ], x - 1.0, y      , z      )),
                                              __perlin_lerp(u, __perlin_grad(g_perm[AB    ], x      , y - 1.0, z      ),
                                                               __perlin_grad(g_perm[BB    ], x - 1.0, y - 1.0, z      ))),
                             __perlin_lerp(v, __perlin_lerp(u, __perlin_grad(g_perm[AA + 1], x      , y      , z - 1.0),
                                                               __perlin_grad(g_perm[BA + 1], x - 1.0, y      , z - 1.0)),
                                              __perlin_lerp(u, __perlin_grad(g_perm[AB + 1], x      , y - 1.0, z - 1.0),
                                                               __perlin_grad(g_perm[BB + 1], x - 1.0, y - 1.0, z - 1.0)))));
}


PERLINAPI double perlin2d(double x, double y) {
    return (__perlin(x, y, 0.0, 0, 0, 0));
}


PERLINAPI double perlin3d(double x, double y, double z) {
    return (__perlin(x, y, z, 0, 0, 0));
}

#  if defined (__cplusplus)

}

#  endif /* __cplusplus */
#
# endif /* PERLIN_IMPLEMENTATION */
#endif /* _perlin_h_ */
