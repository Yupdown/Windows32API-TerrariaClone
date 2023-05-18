#pragma once
#include "CObject.h"

class CTexture;
class CCollider;

class CTile :
    public CObject
{
private:
    const CImage* m_tileImg = {};
    Vec2        m_vBitPos = {};
    Vec2        m_vSliceSize = {};
public:
    CTile();
    ~CTile();
    CTile* Clone()const override
    {
        CTile* newTile = new CTile{ *this };
        return newTile;
    }
    void SetTileTex(wstring_view _strFileName);
    void SetTileBitPos(Vec2 _vBitPos) { m_vBitPos = _vBitPos; }
    void SetTileSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }
    void render(HDC _dc)const override;
    void DeleteTile(HDC _dc);
};

