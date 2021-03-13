#pragma once

#include <cmath>

#define PI 3.14159265358 

class Vec2
{
public:
	float x, y;
	constexpr Vec2(float x, float y) : x(x), y(y) {}

	float operator*(const Vec2& v) const
	{
		return x * v.x + y * v.y;
	}

	void normalize()
	{
		float l = 1 / std::sqrtf(x * x + y * y);
		x *= l;
		y *= l;
	}
};

class Mat2
{
private:
	float a11, a12, a21, a22;
public:
	// rotation matrix with given angle
	Mat2(double angle)
	{
		a11 = std::cos(angle); a12 = -std::sin(angle);
		a21 = std::sin(angle); a22 = std::cos(angle);
	}

	// rotation matrix from unit vector
	Mat2(const Vec2& v)
	{
		a11 = v.x; a12 = -v.y;
		a21 = v.y; a22 = v.x;
	}

	void mul(Vec2& v) const
	{
		float oldvx = v.x;
		v.x = a11 * v.x + a12 * v.y;
		v.y = a21 * oldvx + a22 * v.y;
	}
};