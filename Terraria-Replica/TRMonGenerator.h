#pragma once

class TRMonGenerator
{
private:
	static constexpr Vec2 g_vRandomPos[4]
	{
		Vec2{0,0},
		Vec2{4000,0},
		Vec2{2000,0},
		Vec2{3000,0}
	};
public:
	static void GenerateMonster();
};

