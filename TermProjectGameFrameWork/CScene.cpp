#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"


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

void CScene::AddChunkTile(wstring_view _strTileTexName,Vec2 _vLtPos,Vec2 _vScale ,UINT _iRow, UINT _iCol, UINT _iBitMapIdx)
{
	for (int i = 0; i < (int)_iRow; ++i)
	{
		for (int j = 0; j < (int)_iCol; ++j)
		{
			CTile* pTile = new CTile;
			pTile->SetTileTex(_strTileTexName);
			const double xScale = _vScale.x / static_cast<double>(_iCol);
			const double yScale = _vScale.y / static_cast<double>(_iRow);
			const Vec2 vScale = Vec2{ xScale,yScale };
			const Vec2 vLtPos = Vec2{ _vLtPos.x + xScale *(double)j ,_vLtPos.y + yScale *(double)i};
			pTile->SetPos(vLtPos+vScale/2);
			pTile->SetScale(vScale);
			// x ÇÑÄ­ 64 y ÇÑÄ­ 64
			// x8°³ y4°³
			const UINT yBitPos = _iBitMapIdx / 8;
			const UINT xBitPos = _iBitMapIdx % 8;
			pTile->SetTileSliceSize(Vec2{ 64,64 });
			Vec2 vBit = Vec2{ double(64 * xBitPos),double(64 * yBitPos) };
			pTile->SetTileBitPos(vBit);
			pTile->render(m_pBackGroundTex->GetDC());
			m_mapTile.insert(make_pair(vLtPos + vScale / 2, pTile));
		}
	}

}
void CScene::update()
{
	for (const auto& i : m_vecObj)
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

void CScene::component_update()const
{
	for (const auto& i : m_vecObj)
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
		, static_cast<int>(vCurLook.x)
		, static_cast<int>(vCurLook.y)
		, SRCCOPY);

	std::for_each(std::execution::par, begin(m_vecObj), end(m_vecObj), [](auto& vecObj) {
		auto iter = std::remove_if(std::execution::par, vecObj.begin(), vecObj.end(), [](const auto& pObj) {
			return pObj->IsDead();
			});
		vecObj.erase(iter, vecObj.end());
		});

	for (const auto& vecObj : m_vecObj)
	{
		for (const auto& pObj : vecObj)
		{
			pObj->render(_dc);
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