#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
{
	
}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	wstring wstrCurEXEPath;
	string strCurEXEpath;
	strCurEXEpath.resize(255);
	wstrCurEXEPath.resize(255);
	GetCurrentDirectoryW(255, wstrCurEXEPath.data());
	GetCurrentDirectoryA(255, strCurEXEpath.data());
	auto wstrIdx = wstrCurEXEPath.find(L'\0');
	wstrCurEXEPath.erase(wstrIdx);
	//wstrCurEXEPath[wstrIdx] = L'\0';
	auto strIdx = strCurEXEpath.find('\0');
	strCurEXEpath.erase(strIdx);
	//strCurEXEpath[strIdx] = '\0';
	m_wstrResPath = wstrCurEXEPath + L"\\Output\\Resource\\";
	m_strResPath = strCurEXEpath + "\\Output\\Resource\\";
	m_wstrTexPath = m_wstrResPath + L"Texture";
	m_strSoundPath = m_strResPath + "Sound";
}

