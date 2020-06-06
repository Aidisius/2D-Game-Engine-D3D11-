#pragma once
#include <cmath>


class Vec2DFlt
{
	friend class Vec2DInt;
public:
	// Constructors
	Vec2DFlt() = default;
	Vec2DFlt(float x_in, float y_in)
	{
		x = x_in;
		y = y_in;
	}
public:
	// Operator overloads
	Vec2DFlt operator=(const Vec2DFlt& rhs)
	{
		return rhs;
	}
	Vec2DFlt operator-(const Vec2DFlt& rhs)
	{
		return { x - rhs.x, y - rhs.y };
	}
	Vec2DFlt operator-=(const Vec2DFlt& rhs)
	{
		return *this = *this - rhs;
	}
	Vec2DFlt operator+(const Vec2DFlt& rhs)
	{
		return { x + rhs.x, y + rhs.y };
	}
	Vec2DFlt operator+=(const Vec2DFlt& rhs)
	{
		return *this = *this - rhs;
	}
	Vec2DFlt operator*(const float c)
	{
		return { x * c, y * c };
	}
	Vec2DFlt operator*=(const float c)
	{
		return *this = *this * c;
	}
	Vec2DFlt operator/(const float c)
	{
		return { x / c, y / c };
	}
	Vec2DFlt operator/=(const float c)
	{
		return *this = *this / c;
	}

public:
	// Functions
	float GetX()
	{
		return x;
	}
	float GetY()
	{
		return y;
	}
	void SetX(float x_in)
	{
		x = x_in;
	}
	void SetY(float y_in)
	{
		y = y_in;
	}
	float DistSquaredFromOrigin()
	{
		return x * x + y * y;
	}
	float DistFromOrigin()
	{
		return std::sqrt(DistFromOrigin());
	}
	Vec2DFlt UnitVector()
	{
		float length = DistFromOrigin();
		return *this /= length;
	}
	float DistanceSquaredFromPoint(const Vec2DFlt rhs)
	{
		return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y);
	}
	float DistanceFromPoint(const Vec2DFlt rhs)
	{
		return std::sqrt(DistanceSquaredFromPoint(rhs));
	}
	Vec2DFlt UnitVectorFromPoint(const Vec2DFlt rhs)
	{
		float length = DistanceFromPoint(rhs);
		return { x / length, y / length };
	}

private:
	// Variables
	float x;
	float y;

};


class Vec2DInt
{
	friend class Vec2DFlt;
public:
	// Constructors
	Vec2DInt() = default;
	Vec2DInt(int x_in, int y_in)
	{
		x = x_in;
		y = y_in;
	}
public:
	// Operator overloads
	Vec2DInt operator=(const Vec2DInt& rhs)
	{
		return rhs;
	}
	Vec2DInt operator-(const Vec2DInt& rhs)
	{
		return {x - rhs.x, y - rhs.y};
	}
	Vec2DInt operator-=(const Vec2DInt& rhs)
	{
		return *this = *this - rhs;
	}
	Vec2DInt operator+(const Vec2DInt& rhs)
	{
		return {x + rhs.x, y + rhs.y};
	}
	Vec2DInt operator+=(const Vec2DInt& rhs)
	{
		return *this = *this - rhs;
	}
	Vec2DInt operator*(const int c)
	{
		return {x * c, y * c};
	}
	Vec2DInt operator*=(const int c)
	{
		return *this = *this * c;
	}
	Vec2DInt operator/(const int c)
	{
		return { x / c, y / c };
	}
	Vec2DInt operator/=(const int c)
	{
		return *this = *this / c;
	}
	operator Vec2DFlt()
	{
		return {float(x), float(y)};
	}

public:
	// Functions
	int GetX()
	{
		return x;
	}
	int GetY()
	{
		return y;
	}
	void SetX(int x_in)
	{
		x = x_in;
	}
	void SetY(int y_in)
	{
		y = y_in;
	}
	int DistSquaredFromOrigin()
	{
		return x * x + y * y;
	}
	float DistFromOrigin()
	{
		return std::sqrt(DistFromOrigin());
	}
	
private:
	// Variables
	int x;
	int y;

};
