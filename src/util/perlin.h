#ifndef CT_PERLIN
#define CT_PERLIN

float perlin(float x,float y);

inline float interpolate(float a0, float a1, float w) {
        //return (a1-a0) * w + a0;
        //return (a1-a0) * (3.0 - w * 2.0) * w * w + a0;
        return (a1-a0) * ((w*(w*6.0-15.0)+10.0) * w * w * w) + a0;
}


#endif
