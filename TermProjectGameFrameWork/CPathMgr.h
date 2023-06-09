#pragma once

class CPathMgr
	:public Singleton<CPathMgr>
{
	friend class Singleton;
	CPathMgr();
	~CPathMgr();
private:
	string m_strResPath;
	wstring m_wstrResPath;
	string m_strSoundPath;
	wstring m_wstrTexPath;
public:
	void init();

	const wstring& GetResPathW()const { return m_wstrResPath; }
	const string& GetResPathA()const { return m_strResPath; }

	wstring GetTexFilePath(wstring_view _wstrName)const { return m_wstrResPath + _wstrName.data(); }
	string GetSoundFilePath(string_view _strName)const { return m_strSoundPath + _strName.data(); }

	const wstring& GetTexPath()const { return m_wstrResPath ; }
	const string& GetSoundPath()const { return m_strSoundPath ; }
};


