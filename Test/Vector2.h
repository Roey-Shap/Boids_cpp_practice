#pragma once

#include <math.h>
#include "CustomMath.h"
#include <SFML/Graphics.hpp>

class Vector2
{
public:
	float x;
	float y;
	Vector2 operator+(const Vector2& other);
	Vector2 operator+(const float other);
	Vector2 operator-(const Vector2& other);
	Vector2 operator-(const float other);
	Vector2 operator*(const Vector2& other);
	Vector2 operator*(const float other);
	Vector2 operator/(const Vector2& other);
	Vector2 operator/(const float other);
	Vector2 operator%(const Vector2& other);

	void operator+=(const Vector2& other);
	void operator+=(const float other);
	void operator-=(const Vector2& other);
	void operator-=(const float other);
	void operator*=(const Vector2& other);
	void operator*=(const float other);
	void operator/=(const Vector2& other);
	void operator/=(const float other);

	Vector2* copy();
	float length();
	Vector2 getNormalized();
	void clampLength(float length);
	float angleDegrees();
	float dot(Vector2 other);
	std::string toString();
	Vector2();
	Vector2(float x, float y);
	Vector2(float t);
	Vector2(sf::Vector2f v);
	Vector2(sf::Vector2i v);
	static const Vector2 Vector2Random(const Vector2& min, const Vector2& max);
	static bool pointInRect(Vector2 point, Vector2 topleft, Vector2 bottomRight);
	static bool RectanglesIntersect(Vector2 topLeft1, Vector2 bottomRight1, Vector2 topLeft2, Vector2 bottomRight2);
};