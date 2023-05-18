#pragma once

struct Vec2
{
	float x = {};
	float y = {};

public:

	constexpr bool IsZero()const
	{
		if (x == 0.f && y == 0.f)
		{
			return true;
		}
		return false;
	}
	
	float length()const
	{
		return sqrtf(x * x + y * y);
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

	Vec2 Normalize()const
	{
		float fLen = length();
		if (0. == fLen)
		{
			return Vec2{ 0.f,0.f };
		}
		const float nx = x / fLen;
		const float ny = y / fLen;
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

	constexpr Vec2 operator * (float _f)const
	{
		return Vec2{ x * _f , y * _f };
	}

	constexpr Vec2 operator / (float _f)const
	{
		assert(_f != 0.f);
		return Vec2{ x / _f,y / _f };
	}

	constexpr Vec2& operator*=(float _f)
	{
		(*this) = (*this) * _f;
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