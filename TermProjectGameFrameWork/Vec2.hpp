#pragma once
#include "SimpleMath.hpp"

struct Vec2
{
	float x = {};
	float y = {};

public:
	static const Vec2 zero;
	static const Vec2 one;
	static const Vec2 right;
	static const Vec2 left;
	static const Vec2 up;
	static const Vec2 down;

public:
	bool IsZero() const
	{
		if (IsFloatZero(x) && IsFloatZero(y))
		{
			return true;
		}
		return false;
	}
	
	float length() const
	{
		return Q_sqrt(x * x + y * y);
	}

	constexpr Vec2() = default;

	constexpr Vec2(float _x, float _y)
		:x{ _x }, y{ _y }
	{

	}

	constexpr Vec2(const POINT& p)
		:x{ float(p.x) },
		 y { float(p.y) }
	{

	}

	Vec2 Normalize() const
	{
		const float fLen = length();
		if (IsFloatZero(fLen))
		{
			return Vec2{ 0.f,0.f };
		}
		const float nx = x / fLen;
		const float ny = y / fLen;
		return Vec2{ nx,ny };
	}

	constexpr Vec2 operator+() const
	{
		return Vec2{ x, y };
	}

	constexpr Vec2 operator-() const
	{
		return Vec2{ -x, -y };
	}

	constexpr Vec2 operator+(const Vec2& _other) const
	{
		return Vec2{ x + _other.x , y + _other.y };
	}

	constexpr Vec2 operator-(const Vec2& _other) const
	{
		return Vec2{ x - _other.x , y - _other.y };
	}

	constexpr Vec2 operator*(float _f) const
	{
		return Vec2{ x * _f , y * _f };
	}

	constexpr Vec2 operator/(float _f) const
	{
		assert(_f != 0.f);
		return Vec2{ x / _f,y / _f };
	}

	constexpr Vec2& operator+=(const Vec2& _other)
	{
		(*this) = (*this) + _other;
		return *this;
	}

	constexpr Vec2& operator-=(const Vec2& _other)
	{
		(*this) = (*this) - _other;
		return *this;
	}

	constexpr Vec2& operator*=(float _f)
	{
		(*this) = (*this) * _f;
		return *this;
	}
	
	constexpr Vec2& operator/=(float _f)
	{
		(*this) = (*this) / _f;
		return *this;
	}

	constexpr bool operator==(const Vec2& _other) const
	{
		return x == _other.x && y == _other.y;
	}

	constexpr bool operator!=(const Vec2& _other) const
	{
		return !(*this == _other);
	}
	
	constexpr bool operator<(const Vec2& _v) const
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