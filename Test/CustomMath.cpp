#include <math.h>
#include <random>
#include "CustomMath.h"
#include <SFML/Graphics.hpp>
#include "Vector2.h";
#include <iostream>

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

float randomRange(float min, float max)
{
    return min + negMod(rand(), max);
}

int randomRange(int min, int max)
{
    return min + negMod(rand(), max);
}

sf::Vertex getRegularPolygonVertex(const Vector2& center, float radius, float offsetAngle, int index, int totalPoints)
{
    static const float pi = 3.141592654f;

    float angle = offsetAngle + (index * 2 * pi / totalPoints);
    float x = cos(angle) * radius;
    float y = sin(angle) * radius;

    return sf::Vertex(sf::Vector2f(center.x + x, center.y + y), sf::Color::White);
}