#pragma once

#include <math.h>
class Vector2;
#include <SFML/Graphics.hpp>

float roundTo(float value, float snapValue);
float floorTo(float value, float snapValue);
float ceilTo(float value, float snapValue);
float map(float min1, float max1, float value, float min2, float max2);
float negMod(float x, float y);
float randomRange(float x, float y);
int randomRange(int x, int y);
sf::Vertex getRegularPolygonVertex(const Vector2& center, float radius, float offsetAngle, int index, int totalPoints);