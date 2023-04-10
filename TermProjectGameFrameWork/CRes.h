#pragma once

class CRes
{
private:
	wstring m_wstrKey;
	wstring m_wstrPath;
public:
	void SetKey(wstring_view _wstrKey) { m_wstrKey = _wstrKey; }
	void SetPath(wstring_view _wstrPath) { m_wstrPath = _wstrPath; }
	constexpr const wstring& GetKey()const { return m_wstrKey; }
	constexpr const wstring& GetPath()const { return m_wstrPath; }
public:
	CRes();
	virtual ~CRes();
};
