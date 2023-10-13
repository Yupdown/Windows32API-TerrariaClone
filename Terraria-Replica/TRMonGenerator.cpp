#include "pch.h"
#include "TRMonGenerator.h"
#include "CZombie.h"
#include "CSlime.h"
#include "CEyeMonster.h"
#include "TRWorld.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CTimeMgr.h"

static std::mt19937 rng{std::random_device{}()};
static std::uniform_int_distribution<> uid{0, 3};
extern TRWorld* g_TRWorld;

void TRMonGenerator::GenerateMonster()
{
	auto scene = Mgr(CSceneMgr)->GetScene(SCENE_TYPE::START);
	
	static float fAccTime = 0.f;
	fAccTime += DT;

	if (scene->GetGroupObject(GROUP_TYPE::MONSTER).size() >= 3)
	{
		return;
	}

	if(5.f > fAccTime)
	{
		return;
	}

	fAccTime = 0.f;

	int randNum = uid(rng);
	int randMon = randNum % 2; // randNum % 3;
	const Vec2 vSpawnPos = Vec2(randNum % 2 ? 10 : (float)(TRWorld::WORLD_WIDTH - 10), (float)(TRWorld::WORLD_HEIGHT));
	switch (randMon)
	{
	case 0:
	{
		{
			auto pMon = new CZombie{ g_TRWorld,L"Monster_Zombie",L"NPC_3.png" };
			pMon->SetPos(TRWorld::WorldToGlobal(vSpawnPos));
			pMon->SetScale(Vec2{ 38.0f, 46.0f });
			scene->AddObject(pMon, GROUP_TYPE::MONSTER);
			pMon->SetColliderScale(Vec2{ 38.0f, 46.0f });
		}
	}
	break;
	case 1:
	{
		{
			auto pMon = new CSlime{ g_TRWorld,L"Monster_Slime",L"NPC_1.png" };
			pMon->SetPos(TRWorld::WorldToGlobal(vSpawnPos));
			pMon->SetScale(Vec2{ 32.0f, 24.0f });
			scene->AddObject(pMon, GROUP_TYPE::MONSTER);
			pMon->SetColliderScale(Vec2{ 32.0f, 24.0f });
		}
	}
	break;
	case 2:
	{
		{
			auto pMon = new CEyeMonster{ g_TRWorld,L"Monster_EyeMonster",L"NPC_2.png" };
			pMon->SetPos(TRWorld::WorldToGlobal(Vec2(TRWorld::WORLD_WIDTH / 2 - 10, TRWorld::WORLD_HEIGHT)));
			pMon->SetScale(Vec2{ 38.0f, 22.0f });
			scene->AddObject(pMon, GROUP_TYPE::MONSTER);
			pMon->SetColliderScale(Vec2{ 38.0f, 22.0f });
		}
	}
	break;
	default:
		break;
	}

}
