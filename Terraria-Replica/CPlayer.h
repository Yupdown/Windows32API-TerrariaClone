#pragma once
#include "CObject.h"


class CTexture;
class CAnimation;
class CAnimator;
class TRWorld;
class TRTile;

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    JUMP,
    ATTACK,

    END
};

class CWeapon;

class CPlayer :
    public CObject
{
    friend class CScene_Intro;
private:

    int m_iCurQuickBarIdx = 0;
    vector<CWeapon*>  m_vecWeapon;

    CAnimation* m_pPrevAnim = {};
    unique_ptr<CAnimator> m_pAnimLeg;
    mutable int m_iDegree = 0;
    PLAYER_STATE m_eCurState = PLAYER_STATE::IDLE;
    PLAYER_STATE m_ePrevState = PLAYER_STATE::IDLE;
    bool m_bIsAtk = false;
    bool m_bIsIDLE = false;
    bool m_bPrevCol = false;

    bool m_bSlain = false;
    bool m_bRequestAttack = false;
    float m_fDmgCoolDown = 0.f;
    int m_iMonColCnt = 0;
    HDC m_hPlayerVeilDC; 
    HBITMAP m_hPlayerVeilBit;
public:
    CPlayer(TRWorld* const _trWorld);
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

    void updateQuickBarState(const int _idx);

    void SetSlane(bool _b) { m_bSlain = _b; }
    bool IsPlayerSlain()const { return m_bSlain; }
    void UseItem();
    CoRoutine PlayerRebirthProcess();

    void SetQuickBarIdx(const int _idx) { m_iCurQuickBarIdx = _idx; }
    int GetQuickBarIdx()const { return m_iCurQuickBarIdx; }


    void updateDmgCoolDown();
    void dmg_render(HDC _dc);
    bool IsCanHit()const { return 0.f >= m_fDmgCoolDown || 1.f <= m_fDmgCoolDown; }
};

