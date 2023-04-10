#pragma once

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2	vLT = {}; 
	Vec2	vSlice = {}; 
	Vec2	vOffset = {};
	double	dDuration = {};
};


class CAnimation
{
	friend class CAnimator;
private:
	wstring m_strName = {};
	CAnimator* m_pAnimator = {}; 
	CTexture* m_pTex = {}; 
	vector<tAnimFrm>	m_vecFrm;		
	int					m_iCurFrm = {};
	double				m_dAccTime = {};	
	bool				m_bFinish = false; 
private:
	void SetName(wstring_view _strName) { m_strName = _strName; } 
public:
	CAnimation();
	~CAnimation();
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false; 
		m_iCurFrm = _iFrameIdx; 
		m_dAccTime = 0.; 
	}
	bool	IsFinish()const { return m_bFinish; }
	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
	size_t GetMaxFrame()const { return m_vecFrm.size(); }
	constexpr const wstring& GetName()const { return m_strName; }
public:
	void update();
	void render(HDC _dc,int _iDir);
	void Create(wstring_view _strFileName, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, double _dDuration, UINT _iFrameCount);
public:
	void Save(wofstream& out); 
	bool Load(wifstream& in); 
};

