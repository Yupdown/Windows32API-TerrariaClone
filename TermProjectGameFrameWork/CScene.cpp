#include "pch.h"
#include "CScene.h"
#include "CObject.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::update()
{
	for (auto& i : m_arrObj)
	{
		for (auto& j : i)
		{
			if (j->IsDead())
			{
				continue;
			}
			j->update();
		}
	}
}

void CScene::component_update()
{
	for (auto& i : m_arrObj)
	{
		for (auto& j : i)
		{
			j->component_update();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (auto& i : m_arrObj)
	{
		for (auto iter = i.begin(); iter != i.end();)
		{
			if ((*iter)->IsDead())
			{
				iter->release();
				iter = i.erase(iter);
			}
			else
			{
				(*iter)->render(_dc);
				++iter;
			}
		}
	}
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	m_arrObj[etoi(_eTarget)].clear();
}

void CScene::Reset()
{
	for (auto& i : m_arrObj)
	{
		i.clear();
	}
}
