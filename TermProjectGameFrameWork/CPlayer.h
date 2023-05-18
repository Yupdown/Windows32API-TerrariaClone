#pragma once
#include "CObject.h"

class CTexture;
class CAnimation;
class CAnimator;

class CPlayer :
    public CObject
{
private:
    const CImage* m_playerImg = {};
    CAnimation* m_pPrevAnim = {};
    unique_ptr<CAnimator> m_pAnimLeg;
    mutable int m_iDegree = 0;
public:
    CPlayer();
    CPlayer(const CPlayer& other);
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

