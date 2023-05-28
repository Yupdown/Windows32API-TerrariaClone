#pragma once

#include <string>
#include <Windows.h>
#include "CAtlasElement.h"

class TRItem
{
protected:
	std::wstring name;
	std::wstring k_element;

public:
	TRItem();
	~TRItem();
};