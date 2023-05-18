#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamera.h"
#include "CEventMgr.h"

int CScene::g_iSceneID;

CScene::CScene()
{
	
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
	//pTile->render(m_pBackGroundTex->GetDC());
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
			const float xScale = _vScale.x / static_cast<float>(_iCol);
			const float yScale = _vScale.y / static_cast<float>(_iRow);
			const Vec2 vScale = Vec2{ xScale,yScale };
			const Vec2 vLtPos = Vec2{ _vLtPos.x + xScale *(float)j ,_vLtPos.y + yScale *(float)i};
			pTile->SetPos(vLtPos+vScale/2);
			pTile->SetScale(vScale);
			// x ÇÑÄ­ 64 y ÇÑÄ­ 64
			// x8°³ y4°³
			const UINT yBitPos = _iBitMapIdx / 8;
			const UINT xBitPos = _iBitMapIdx % 8;
			pTile->SetTileSliceSize(Vec2{ 64,64 });
			Vec2 vBit = Vec2{ float(64 * xBitPos),float(64 * yBitPos) };
			pTile->SetTileBitPos(vBit);
		//	pTile->render(m_pBackGroundTex->GetDC());
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
	const Vec2 vRes = Mgr(CCore)->GetResolutionV();
	Mgr(CCamera)->renderBackGround(m_pBackGroundImg, m_pBackGroundImg2,1,10);
	Mgr(CCamera)->SetNowLookAt(Mgr(CCore)->GetResolutionV() / 2);
	Mgr(CCamera)->TransformRenderPos();
	m_pBackGroundImg2->BitBlt(_dc
		, 0
		, 0
		, (int)vRes.x
		, (int)vRes.y
		, 0
		, 0);
	Mgr(CCamera)->ResetRenderPos();
	Mgr(CCamera)->SetNowLookAt(GetPlayer()->GetPos());
	
	for (auto& vecObj : m_vecObj)
	{
		const auto vecPtr = vecObj.data();
		for (size_t i = 0; i < vecObj.size();)
		{
			if (vecPtr[i]->IsDead())
			{
				//m_vecDeadObj.emplace_back(std::move(vecPtr[i]));
				Mgr(CEventMgr)->AddDeadObj(vecPtr[i]);
				vecPtr[i] = std::move(vecObj.back());
				vecObj.pop_back();
			}
			else
			{
				vecPtr[i]->render(_dc);
				++i;
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
