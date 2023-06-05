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