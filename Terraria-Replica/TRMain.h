#pragma once

#include "TRWorld.h"

class TRMain
{
private:
	TRWorld* active_world;

public:
	TRMain();
	~TRMain();

	void Update();
};

