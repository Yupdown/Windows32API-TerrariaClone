#pragma once
#include "CObject.h"

class CTexture;
class CAnimation;
class CAnimator;

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    JUMP,
    ATTACK,

    END
};

class CPlayer :
    public CObject
{
private:
    CAnimation* m_pPrevAnim = {};
    unique_ptr<CAnimator> m_pAnimLeg;
    mutable int m_iDegree = 0;
    PLAYER_STATE m_eCurState = PLAYER_STATE::IDLE;
    PLAYER_STATE m_ePrevState = PLAYER_STATE::IDLE;
    bool m_bIsAtk = false;
    bool m_bIsIDLE = false;
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
    void updateState();
    void updateMove();
    void updateAnimation();
    void component_update()override;
    virtual void OnCollision(CCollider* const _pOther);
    virtual void OnCollisionEnter(CCollider* const _pOther);
    virtual void OnCollisionExit(CCollider* const _pOther);

};

