#include "pch.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CCore.h"

CTile::CTile()
{
	m_bIsCamAffected = false;
}

CTile::~CTile()
{
}

void CTile::SetTileTex(wstring_view _strFileName)
{
	m_tileImg = Mgr(CResMgr)->GetImg(_strFileName);
}

void CTile::render(HDC _dc)const
{

}

void CTile::DeleteTile(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Vec2 vLt = vPos - vScale / 2;
	Vec2 vRb = vPos + vScale / 2;
	vLt.x = max(vLt.x, 0);
	vLt.y = max(vLt.y, 0);
	PatBlt(_dc
		, (int)vLt.x
		, (int)vLt.y
		, (int)vScale.x
		, (int)vScale.y
		, WHITENESS);
}
