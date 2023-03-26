#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"

CTexture* CScene::m_pBackGroundTex;

CScene::CScene()
{
	m_pBackGroundTex = Mgr(CResMgr)->CreateTexture(L"SceneBackGround", Mgr(CCore)->GetResolution().x * 10, Mgr(CCore)->GetResolution().y * 10);
	Clear();
}

CScene::~CScene()
{
}

void CScene::AddTile(wstring_view _strTileTexName, Vec2 _vPos, Vec2 _vScale, Vec2 _vBitPos, Vec2 _vSlice)
{
	CTile* pTile = new CTile;
	pTile->SetTileTex(_strTileTexName);
	pTile->SetPos(_vPos);
	pTile->SetScale(_vScale);
	pTile->SetTileBitPos(_vBitPos);
	pTile->SetTileSliceSize(_vSlice);
	pTile->render(m_pBackGroundTex->GetDC());
	m_mapTile.insert(make_pair(_vPos, pTile));
}

void CScene::update()
{
	for (auto& i : m_vecObj)
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
	for (auto& i : m_vecObj)
	{
		for (auto& j : i)
		{
			j->component_update();
		}
	}
}

void CScene::render(HDC _dc)
{
	Vec2 vResolution = Mgr(CCore)->GetResolution();
	Vec2 vCurLook = Mgr(CCamera)->GetLookAt() - vResolution / 2.;

	BitBlt(_dc
		, 0
		, 0
		, Mgr(CCore)->GetResolution().x
		, Mgr(CCore)->GetResolution().y
		, m_pBackGroundTex->GetDC()
		, std::lround(vCurLook.x)
		, std::lround(vCurLook.y)
		, SRCCOPY);

	for (auto& i : m_vecObj)
	{
		for (auto iter = i.begin(); iter != i.end();)
		{
			if ((*iter)->IsDead())
			{
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
	m_vecObj[etoi(_eTarget)].clear();
}

void CScene::Reset()
{
	for (auto& i : m_vecObj)
	{
		i.clear();
	}
}

void CScene::Clear()
{
	PatBlt(m_pBackGroundTex->GetDC(), 0, 0, Mgr(CCore)->GetResolution().x * 10, Mgr(CCore)->GetResolution().y * 10, WHITENESS);
}