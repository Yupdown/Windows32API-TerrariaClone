#pragma once

inline short bitwise_abs_short(const short x)
{
	const short mask = x >> (sizeof(short) * 8 - 1);
	return (x + mask) ^ mask;
}

inline float bitwise_absf(float num)
{
	int& numAsInt = reinterpret_cast<int&>(num);
	numAsInt &= 0x7FFFFFFF;
	return num;
}

inline int bitwise_absi(const int num)
{
	int mask = num >> (sizeof(int) * 8 - 1);
	return (num + mask) ^ mask;
}

inline float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                    
	i = 0x5f3759df - (i >> 1);            
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y)); 
	
	return y;
}

inline bool IsFloatZero(float number)
{
	return (number >= -FLT_EPSILON && number <= FLT_EPSILON);
}

inline float Q_sqrt(float number)
{
	const float invSqrt = Q_rsqrt(number);
	return IsFloatZero(number) ? 0.f : 1.0f / invSqrt;
}