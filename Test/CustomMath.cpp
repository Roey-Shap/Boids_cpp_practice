#include <math.h>

float roundTo(float value, float snapValue)
{
    return (roundf(value / snapValue) * snapValue);
}

float floorTo(float value, float snapValue)
{
    return (floorf(value / snapValue) * snapValue);
}

float ceilTo(float value, float snapValue)
{
    return (ceilf(value / snapValue) * snapValue);
}

float map(float min1, float max1, float value, float min2, float max2)
{
    float d2 = max2 - min2;
    float d1 = max1 - min1;
    float sizeRatio = d2 / d1;

    return (value * sizeRatio) - (min1 * sizeRatio) + min2;
}


float negMod(float x, float y)
{
    float remainder = fmodf(x, y);
    remainder = remainder > 0 ? remainder : y + remainder;
    return remainder;
}