#pragma once
#include "CObject.h"

class CTexture;
class CAnimation;
class CPlayer :
    public CObject
{
private:
    CTexture* m_pTex;
    CAnimation* m_pPrevAnim = {};
public:
    CPlayer();
    void update()override;
    void render(HDC _dc)const override;
    CPlayer* Clone()const override
    {
        auto p = new CPlayer{ *this };
        return p;
    }
    virtual void OnCollision(CCollider* const _pOther);
    virtual void OnCollisionEnter(CCollider* const _pOther);
    virtual void OnCollisionExit(CCollider* const _pOther);

};

