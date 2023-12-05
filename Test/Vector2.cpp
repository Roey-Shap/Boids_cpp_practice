#include "Vector2.h"
#include <math.h>
#include <random>

using namespace std;

/*
	float length();
	Vector2 getNormalized();*/


Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2()
{
	x = 0.0;
	y = 0.0;
}

Vector2 Vector2::operator+(const Vector2& other)
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator+(const float other)
{
	return Vector2(x + other, y + other);
}

Vector2 Vector2::operator-(const Vector2& other)
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator-(const float other)
{
	return Vector2(x - other, y - other);
}

Vector2 Vector2::operator*(const Vector2& other)
{
	return Vector2(x * other.x, y * other.y);
}

Vector2 Vector2::operator*(const float other)
{
	return Vector2(x * other, y * other);
}

Vector2 Vector2::operator/(const Vector2& other)
{
	return Vector2(x / other.x, y / other.y);
}

Vector2 Vector2::operator/(const float other)
{
	return Vector2(x / other, y / other);
}

Vector2 Vector2::operator%(const Vector2& other)
{
	return Vector2(negMod(x, other.x), negMod(y, other.y));
}

void Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
}

void Vector2::operator+=(const float other)
{
	x += other;
	y += other;
}

void Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
}

void Vector2::operator-=(const float other)
{
	x -= other;
	y -= other;
}

void Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;
}

void Vector2::operator*=(const float other)
{
	x *= other;
	y *= other;
}

void Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;
}

void Vector2::operator/=(const float other)
{
	x /= other;
	y /= other;
}

float Vector2::length()
{
	return sqrtf(abs(x * x) + abs(y * y));
}

Vector2 Vector2::getNormalized()
{
	float len = length();
	if (len > 0)
	{
		return (*this) / len;
	}
	else
	{
		return (*this);
	}
}

void Vector2::clampLength(float length)
{
	if (this->length() > length)
	{
		Vector2 clamped = getNormalized() * length;
		x = clamped.x;
		y = clamped.y;
	}
}

Vector2* Vector2::copy()
{
	return new Vector2(x, y);
}

const Vector2 Vector2::Vector2Random(const Vector2& min, const Vector2& max)
{
	return Vector2(min.x + negMod(rand(), max.x), min.y + negMod(rand(), max.y));
}

bool Vector2::pointInRect(Vector2* point, Vector2* topLeft, Vector2* bottomRight)
{
	return point->x <= bottomRight->x && point->x >= topLeft->x && point->y <= bottomRight->y && point->y >= topLeft->y;
}

bool Vector2::RectanglesIntersect(Vector2* topLeft1, Vector2* bottomRight1, Vector2* topLeft2, Vector2* bottomRight2)
{
	return topLeft1->x <= bottomRight2->x && bottomRight1->x >= topLeft2->x 
		&& topLeft1->y <= bottomRight2->y && bottomRight1->y >= topLeft2->y;
}