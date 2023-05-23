#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CObject.h"
#include "CTexture.h"
#include "CTimeMgr.h"
//#include "CCamera.h"	// �������� ��ġ�� ī�޶� �������ش�.
//#include "SelectGDI.h"
#include "CPathMgr.h"
#include "CResMgr.h"

template<typename T>
void ReadTxtSkipWs(wifstream& in, T& out)
{
	in >> out;
}

template<typename... outPuts>
void ReadMany(wifstream& in, outPuts&... out)
{
	static wstring dummy;
	getline(in, dummy);

	(ReadTxtSkipWs(in, out), ...);

	while ('\n' == in.peek())in.get();
}

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	m_fAccTime += DT;
	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;
		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = 0;
			m_bFinish = true;
		}
	}
}

void CAnimation::render(HDC _dc, int _iDir)
{
	if (m_bFinish)
	{
		//return;
	}
	CObject* pObj = m_pAnimator->GetOwner();
	Vec2 vPos = pObj->GetPos();
	vPos += m_vecFrm[m_iCurFrm].vOffset;

	//vPos = CCamera::GetInst()->GetRenderPos(vPos);
	//Mgr(CResMgr)->renderImg(m_pAnimImg, pObj, m_vecFrm[m_iCurFrm].vLT, m_vecFrm[m_iCurFrm].vSlice);
	//m_pTex->render(_dc, m_pAnimator->GetOwner(), m_vecFrm[m_iCurFrm].vLT , m_vecFrm[m_iCurFrm].vSlice);

	Mgr(CResMgr)->renderImg(m_pAnimImg, m_pAnimator->GetOwner(), m_vecFrm[m_iCurFrm].vLT + Vec2{ m_vecFrm[m_iCurFrm].vSlice.x *_iDir,0 }, m_vecFrm[m_iCurFrm].vSlice - Vec2{ m_vecFrm[m_iCurFrm].vSlice.x*2*_iDir, 0});
	
	//m_pTex->render(_dc, m_pAnimator->GetOwner(), m_vecFrm[m_iCurFrm].vLT + Vec2{ m_vecFrm[m_iCurFrm].vSlice.x *_iDir,0 }, m_vecFrm[m_iCurFrm].vSlice - Vec2{ m_vecFrm[m_iCurFrm].vSlice.x*2*_iDir, 0});
	
	
	/*TransparentBlt(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.)
		, (int)m_vecFrm[m_iCurFrm].vSlice.x
		, (int)m_vecFrm[m_iCurFrm].vSlice.y
		, m_pTex->GetDC()
		, (int)m_vecFrm[m_iCurFrm].vLT.x
		, (int)m_vecFrm[m_iCurFrm].vLT.y
		, (int)m_vecFrm[m_iCurFrm].vSlice.x
		, (int)m_vecFrm[m_iCurFrm].vSlice.y
		, RGB(255, 0, 255)
	);*/

	//BLENDFUNCTION bf = {};

	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 255; // ���� ���İ�

	//AlphaBlend(_dc // ������ dc
	//	, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)		// �»��
	//	, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
	//	, (int)m_vecFrm[m_iCurFrm].vSlice.x						// �����̽� ������
	//	, (int)m_vecFrm[m_iCurFrm].vSlice.y
	//	, m_pTex->GetDC()		// ������ �����ִ� DC
	//	, (int)m_vecFrm[m_iCurFrm].vLT.x		// ������������ �»��
	//	, (int)m_vecFrm[m_iCurFrm].vLT.y
	//	, (int)m_vecFrm[m_iCurFrm].vSlice.x		// ������������ �����̽� ������
	//	, (int)m_vecFrm[m_iCurFrm].vSlice.y
	//	, bf
	//);	// ���������� ����ȭ �� ����

}

void CAnimation::Create(wstring_view _strFileName, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pAnimImg = Mgr(CResMgr)->GetImg(_strFileName);
	assert(m_pAnimImg);
	tAnimFrm frm = {}; 
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;	
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * static_cast<float>(i); 
		m_vecFrm.emplace_back(frm);
	}
}

void CAnimation::Save(wofstream& out)
{
	//wstring strFilePath = Mgr(CPathMgr)->GetContentPath();
	//strFilePath += _strRelativePath;

	//wofstream out{ strFilePath };
	//assert(out);

	// 1. �ִϸ��̼��� �̸��� �����Ѵ�.
	out << L"[Animation Name]" << '\n';
	out << m_strName << '\n' << '\n';

	// 2. �ؽ����� Ű���� ��θ� �����Ѵ�.
	out << L"[Texture Name]" << '\n';
	//out << m_pTex->GetKey() << '\n' << '\n';

	out << L"[Texture Path]" << '\n';
	//out << m_pTex->GetPath() << '\n' << '\n';

	// 3. ������ ����
	out << L"[Frame Count]" << '\n';
	out << m_vecFrm.size() << '\n' << '\n';

	// 4. ������ ����
	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		out << L"[Frame Index]" << '\n';
		out << i << '\n' << '\n';

		out << L"[Left Top]" << '\n';
		out << m_vecFrm[i].vLT.x << ' ' << m_vecFrm[i].vLT.y << '\n' << '\n';

		out << L"[Slice Size]" << '\n';
		out << m_vecFrm[i].vSlice.x << ' ' << m_vecFrm[i].vSlice.y << '\n' << '\n';

		out << L"[OffSet]" << '\n';
		out << m_vecFrm[i].vOffset.x << ' ' << m_vecFrm[i].vOffset.y << '\n' << '\n';

		out << L"[Duration]" << '\n';
		out << m_vecFrm[i].fDuration << '\n';

		out << '\n' << '\n';
	}
}

bool CAnimation::Load(wifstream& in)
{
	//wstring strFilePath = Mgr(CPathMgr)->GetContentPath();
	//strFilePath += _strRelativePath;

	//wifstream in{ strFilePath };

	static wstring dummy;
	//dummy.reserve(256);
	while ('\n' == in.peek())in.get();
	// 1. �ִϸ��̼��� �̸��� �о�´�.
	getline(in, dummy); // �տ� ����
	if (!in)
	{
		in.close();
		return false;
	}
	in >> m_strName;
	while ('\n' == in.peek())in.get();
	// 2. �ؽ����� Ű��(�̸�) �� ��θ� �޾ƿͼ� �ε��ؽ��ķ� �ε��Ѵ�
	//getline(in, dummy);
	wstring strTexKey;
	//in >> strTexKey;
	//while ('\n' == in.peek())in.get();
	ReadMany(in, strTexKey);

	//getline(in, dummy);
	wstring strTexPath;
	//in >> strTexPath;
	//while ('\n' == in.peek())in.get();
	ReadMany(in, strTexPath);
	// �̸��� ��θ� �������� �ؽ��ĸ� ���� �����Ϳ� �����Ѵ�
	//m_pTex = Mgr(CResMgr)->LoadTexture(strTexKey, strTexPath);

	// 3. ������ ����
	size_t iFrameCount = 0;
	//getline(in, dummy);
	//in >> iFrameCount;
	//while ('\n' == in.peek())in.get();
	ReadMany(in, iFrameCount);
	assert(iFrameCount);
	m_vecFrm.resize(iFrameCount);

	for (auto& frmInfo : m_vecFrm)
	{
		//getline(in, dummy);
		int iData = 0;
		//in >> iData;	// �տ� ������ ��ȣ
		//while ('\n' == in.peek())in.get();
		ReadMany(in, iData);

		//getline(in, dummy); // Left Top
		//in >> frmInfo.vLT.x >> frmInfo.vLT.y;
		//while ('\n' == in.peek())in.get();
		ReadMany(in, frmInfo.vLT.x, frmInfo.vLT.y);

		//getline(in, dummy); // Slice Size;
		//in >> frmInfo.vSlice.x >> frmInfo.vSlice.y;
		//while ('\n' == in.peek())in.get();
		ReadMany(in, frmInfo.vSlice.x, frmInfo.vSlice.y);

		//getline(in, dummy); // OffSet
		//in >> frmInfo.vOffset.x >> frmInfo.vOffset.y;
		//while ('\n' == in.peek())in.get();
		ReadMany(in, frmInfo.vOffset.x, frmInfo.vOffset.y);

		//getline(in, dummy); // duration
		//in >> frmInfo.fDuration;
		//while ('\n' == in.peek())in.get();
		ReadMany(in, frmInfo.fDuration);
	}
	//while ('\n' == in.peek())in.get();
	return true;
}
