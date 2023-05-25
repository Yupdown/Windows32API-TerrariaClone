#pragma once

#include <Windows.h>
#include <math.h>
#include "Vec2.hpp"

struct Vec2Int
{
public:
	int x;
	int y;

public:
	static const Vec2Int zero;
	static const Vec2Int one;
	static const Vec2Int right;
	static const Vec2Int left;
	static const Vec2Int up;
	static const Vec2Int down;

public:
	constexpr Vec2Int(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	constexpr Vec2Int() : Vec2Int(0, 0)
	{

	}

	constexpr Vec2Int(const Vec2& _vec) : Vec2Int(static_cast<int>(floorf(_vec.x)), static_cast<int>(floorf(_vec.y)))
	{

	}
	
	constexpr Vec2Int(const POINT& _point) : Vec2Int(_point.x, _point.y)
	{

	}

	constexpr Vec2Int(const SIZE& _size) : Vec2Int(_size.cx, _size.cy)
	{

	}

	constexpr Vec2Int operator+() const
	{
		return Vec2Int(x, y);
	}

	constexpr Vec2Int operator-() const
	{
		return Vec2Int(-x, -y);
	}

	constexpr Vec2Int operator+(const Vec2Int& _other) const
	{
		return Vec2Int{ x + _other.x , y + _other.y };
	}

	constexpr Vec2Int operator-(const Vec2Int& _other) const
	{
		return Vec2Int{ x - _other.x , y - _other.y };
	}

	constexpr Vec2Int operator*(int _f) const
	{
		return Vec2Int{ x * _f, y * _f };
	}

	constexpr Vec2Int operator/(int _f) const
	{
		return Vec2Int{ x / _f, y / _f };
	}

	constexpr Vec2Int& operator+=(const Vec2Int& _other)
	{
		return *this = *this + _other;
	}

	constexpr Vec2Int& operator-=(const Vec2Int& _other)
	{
		return *this = *this - _other;
	}

	constexpr Vec2Int& operator*=(int _f)
	{
		return *this = *this * _f;
	}

	constexpr Vec2Int& operator/=(int _f)
	{
		return *this = *this / _f;
	}

	constexpr bool operator==(const Vec2Int& _other) const
	{
		return x == _other.x && y == _other.y;
	}

	constexpr bool operator!=(const Vec2Int& _other) const
	{
		return !(*this == _other);
	}

	constexpr operator Vec2() const
	{
		return Vec2(static_cast<float>(x), static_cast<float>(y));
	}

	constexpr operator POINT() const
	{
		return POINT{ x, y };
	}

	constexpr operator SIZE() const
	{
		return SIZE{ x, y };
	}
};