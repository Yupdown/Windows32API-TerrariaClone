#pragma once
#include "CComponent.h"

class CObject;
class CTexture;
class CAnimation;

class CAnimator
	:public CComponent
{
	CAnimator(const CAnimator& _other);
public:
	CAnimator();
	~CAnimator();
private:
	unordered_map<wstring, unique_ptr<CAnimation>>	m_mapAnim; 
	CAnimation* m_pCurAnim = {};	
	bool			m_bRepeat = false; 
	int m_iAnimDir = {};
	wifstream			m_fsIn;
	wofstream			m_fsOut;
public:
	void CreateAnimation(wstring_view _strAnimName, wstring_view _strFileName, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep , float _fDuration, UINT _iFrameCount);
	optional<CAnimation*> FindAnimation(wstring_view _strName);
	void Play(wstring_view _strName, bool _bRepeat);	
	void component_update()override;
	void component_render(HDC _dc)const override;
	void SetAnimRight() { m_iAnimDir = 0; }
	void SetAnimLeft() { m_iAnimDir = 1; }
	bool IsFinish()const;
	CAnimation* GetCurAnim()const { return m_pCurAnim; };
public:
	CAnimator* Clone(CObject* const _pOwner)const override
	{
		CAnimator* pNewComp = new CAnimator{ *this };
		pNewComp->SetOwner(_pOwner);
		return pNewComp;
	}
	int GetAnimDir()const { return m_iAnimDir; }
	void LoadAnimation(wstring_view _strRelativePath);
	void SaveAnimation(wstring_view _strRelativePath);
};



