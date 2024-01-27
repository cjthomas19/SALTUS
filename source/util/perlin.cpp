#include <math.h>
#include "perlin.h"

v2 randomUnitVector(int ix, int iy) {
    const unsigned int w = 8 * sizeof(unsigned int);
    const unsigned int s = w / 2;
    unsigned int a = ix;
    unsigned int b = iy;
    a*= 3284157443; b^=a<<s | a>>w-s;
    b*= 1911520717; a^=b<<s | b>>w-s;
    a*= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1));
    v2 v;
    v.x = cos(random);
    v.y = sin(random);
    return v;
}

float dotGridGradient(int ix, int iy, float x, float y) {
    v2 gradient = randomUnitVector(ix,iy);
    float dx = x-(float)ix;
    float dy = y-(float)iy;

    return (dx * gradient.x) + (dy*gradient.y);
}

float perlin(float x,float y) {
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    float sx = x-(float)x0;
    float sy = y-(float)y0;

    float n0,n1,ix0,ix1,value;

    n0 = dotGridGradient(x0,y0,x,y);
    n1 = dotGridGradient(x1,y0,x,y);
    ix0 = interpolate(n0,n1,sx);

    n0 = dotGridGradient(x0,y1,x,y);
    n1 = dotGridGradient(x1,y1,x,y);
    ix1 = interpolate(n0,n1,sx);

    value = interpolate(ix0,ix1,sy);

    return value;
}