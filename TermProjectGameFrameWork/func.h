#pragma once

template <typename T>
constexpr int etoi(T _eType) { return static_cast<int>(_eType); }

int wrapAround(int x, int low, int high);