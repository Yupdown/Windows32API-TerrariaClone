#pragma once

struct Vec2
{
	double x = {};
	double y = {};

public:

	bool IsZero()const
	{
		if (x == 0. && y == 0.)
		{
			return true;
		}
		return false;
	}
	
	double length()const
	{
		return sqrt(x * x + y * y);
	}

	Vec2() = default;

	Vec2(double _x, double _y)
		:x{ _x }, y{ _y }
	{

	}

	Vec2(const POINT& p)
		:x{ double(p.x) },
		y { double(p.y) }
	{

	}

	Vec2 Normalize()
	{
		double dLen = length();
		if (0. == dLen)
		{
			return Vec2{ 0.,0. };
		}
		const double nx = x / dLen;
		const double ny = y / dLen;
		return Vec2{ nx,ny };
	}

	Vec2 operator + (const Vec2 _other)const
	{
		return Vec2{ x + _other.x , y + _other.y };
	}

	Vec2& operator +=(const Vec2 _other)
	{
		(*this) = (*this) + _other;
		return *this;
	}

	Vec2 operator - (const Vec2 _other)const
	{
		return Vec2{ x - _other.x , y - _other.y };
	}

	Vec2 operator * (const Vec2 _other)const
	{
		return Vec2{ x * _other.x , y * _other.y };
	}

	Vec2 operator * (double _i)const
	{
		return Vec2{ x * (double)_i , y * (double)_i };
	}

	Vec2 operator / (double _d)const
	{
		if (0. == _d)
		{
			return Vec2{ 0.,0. };
		}
		return Vec2{ x / _d,y / _d };
	}

	Vec2& operator*=(double _d)
	{
		(*this) = (*this) * _d;
		return *this;
	}

	Vec2& operator-=(const Vec2& _v)
	{
		(*this) = (*this) - _v;
		return *this;
	}
	
};