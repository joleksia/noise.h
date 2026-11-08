#if defined (__cplusplus)
# pragma once
#endif /* __cplusplus */
#
#if !defined (_perlin_h_)
# define _perlin_h_ 1
#
# if defined (__cplusplus)

extern "C" {

# endif /* __cplusplus */

/* public API */

double perlin2d(double, double, int);

double perlin3d(double, double, double, int);

double fbm2d(double, double, double, double, int, int);

double fbm3d(double, double, double, double, double, int, int);

double wrap2d(double, double, int, int, int);

double wrap3d(double, double, double, int, int, int, int);

# if defined (__cplusplus)

}

# endif /* __cplusplus */
#endif /* _perlin_h_ */
#
#if defined (NOISE_IMPLEMENTATION)
# if !defined (_perlin_impl_h_)
#  define _perlin_impl_h_ 1
#
#  include <stddef.h>
#  include <stdint.h>
#
#  if defined (__cplusplus)

extern "C" {

#  endif /* __cplusplus */

/* static API */

static const uint8_t _p_perm[512] = {
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


static inline double __p_floor(double a) {
    return ((a < (int) a) ? (int) a - 1 : (int) a);
}


static inline double __p_fade(double t) {
    return (t * t * t * (t * (t * 6.0 - 15.0) + 10.0));
}


static inline double __p_lerp(double a, double b, double t) {
    return (a + t * (b - a));
} 


static inline double __p_grad(int hash, double x, double y, double z) {
    int h = hash & 0x0f;
    double u = h < 8 ? x : y,
           v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return (((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v));
}


static inline double __p_int(double x, double y, double z, int x_wrap, int y_wrap, int z_wrap, unsigned char seed) {
    uint32_t xm = (x_wrap - 1) & 0xff,
             ym = (y_wrap - 1) & 0xff,
             zm = (z_wrap - 1) & 0xff;

    int32_t xf = __p_floor(x),
            yf = __p_floor(y),
            zf = __p_floor(z);

    int32_t x0 = xf & xm, x1 = xf + 1 & xm,
            y0 = yf & ym, y1 = yf + 1 & ym,
            z0 = zf & zm, z1 = zf + 1 & zm;

    int32_t p0 = _p_perm[_p_perm[x0] + seed],
            p1 = _p_perm[_p_perm[x1] + seed];

    int32_t p00 = _p_perm[p0 + y0], p01 = _p_perm[p0 + y1],
            p10 = _p_perm[p1 + y0], p11 = _p_perm[p1 + y1];

    x -= xf, y -= yf, z -= zf;
    double u = __p_fade(x),
           v = __p_fade(y),
           w = __p_fade(z);

    double g000 = __p_grad(_p_perm[p00 + z0], x,     y,     z    ),
           g001 = __p_grad(_p_perm[p00 + z1], x,     y,     z - 1),
           g010 = __p_grad(_p_perm[p01 + z0], x,     y - 1, z    ),
           g011 = __p_grad(_p_perm[p01 + z1], x,     y - 1, z - 1),
           g100 = __p_grad(_p_perm[p10 + z0], x - 1, y,     z    ),
           g101 = __p_grad(_p_perm[p10 + z1], x - 1, y,     z - 1),
           g110 = __p_grad(_p_perm[p11 + z0], x - 1, y - 1, z    ),
           g111 = __p_grad(_p_perm[p11 + z1], x - 1, y - 1, z - 1);

    double l00 = __p_lerp(g000, g001, w),
           l01 = __p_lerp(g010, g011, w),
           l10 = __p_lerp(g100, g101, w),
           l11 = __p_lerp(g110, g111, w);

    double l0 = __p_lerp(l00, l01, v),
           l1 = __p_lerp(l10, l11, v);

    return (__p_lerp(l0, l1, u));
}

/* public API */

double perlin2d(double x, double y, int seed) {
    return (__p_int(x, y, 0.0, 0, 0, 0, seed));
}


double fbm2d(double x, double y, double lacunarity, double persistance, int octaves, int seed) {
    double result = 0.0;
    double amplitude = 0.5,
           frequency = 1.0;
    for (int i = 0; i < octaves; i++) {
        result += amplitude * __p_int(x * frequency,
                                      y * frequency,
                                      0.0, 0, 0, 0,
                                      seed);
        frequency *= lacunarity;
        amplitude *= persistance;
    }
    
    return (result);
}


double perlin3d(double x, double y, double z, int seed) {
    return (__p_int(x, y, z, 0, 0, 0, seed));
}


double fbm3d(double x, double y, double z, double lacunarity, double persistance, int octaves, int seed) {
    double result = 0.0;
    double amplitude = 0.5,
           frequency = 1.0;
    for (int i = 0; i < octaves; i++) {
        result += amplitude * __p_int(x * frequency,
                                      y * frequency,
                                      z * frequency,
                                      0, 0, 0,
                                      seed);
        frequency *= lacunarity;
        amplitude *= persistance;
    }
    
    return (result);
}


double wrap2d(double x, double y, int x_wrap, int y_wrap, int seed) {
    return (__p_int(x, y, 0.0, x_wrap, y_wrap, 0, seed));
}


double wrap3d(double x, double y, double z, int x_wrap, int y_wrap, int z_wrap, int seed) {
    return (__p_int(x, y, z, x_wrap, y_wrap, z_wrap, seed));
}

#  if defined (__cplusplus)

}

#  endif /* __cplusplus */
#
# endif /* NOISE_IMPLEMENTATION */
#endif /* _perlin_impl_h_ */
