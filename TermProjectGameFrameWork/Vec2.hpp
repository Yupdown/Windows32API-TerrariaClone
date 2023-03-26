#pragma once

struct Vec2
{
	double x = {};
	double y = {};

public:

	constexpr bool IsZero()const
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

	constexpr Vec2() = default;

	constexpr Vec2(double _x, double _y)
		:x{ _x }, y{ _y }
	{

	}

	constexpr Vec2(const POINT& p)
		:x{ double(p.x) },
		y { double(p.y) }
	{

	}

	Vec2 Normalize()const
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

	constexpr Vec2 operator + (const Vec2& _other)const
	{
		return Vec2{ x + _other.x , y + _other.y };
	}

	constexpr Vec2& operator +=(const Vec2& _other)
	{
		(*this) = (*this) + _other;
		return *this;
	}

	constexpr Vec2 operator - (const Vec2& _other)const
	{
		return Vec2{ x - _other.x , y - _other.y };
	}

	constexpr Vec2 operator * (const Vec2& _other)const
	{
		return Vec2{ x * _other.x , y * _other.y };
	}

	constexpr Vec2 operator * (double _i)const
	{
		return Vec2{ x * (double)_i , y * (double)_i };
	}

	constexpr Vec2 operator / (double _d)const
	{
		assert(_d != 0.);
		return Vec2{ x / _d,y / _d };
	}

	constexpr Vec2& operator*=(double _d)
	{
		(*this) = (*this) * _d;
		return *this;
	}

	constexpr Vec2& operator-=(const Vec2& _v)
	{
		(*this) = (*this) - _v;
		return *this;
	}
	
public:
	constexpr bool operator <(const Vec2& _v)const
	{
		if (y == _v.y)
		{
			return x < _v.x;
		}
		else
		{
			return y < _v.y;
		}
	}
	operator POINT()const
	{
		POINT temp{};
		temp.x = std::lround(x);
		temp.y = std::lround(y);
		return temp;
	}
};