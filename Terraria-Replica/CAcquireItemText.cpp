#include "pch.h"
#include "CAcquireItemText.h"
#include "TRItemStack.h"
#include "TRItem.h"
#include "CCamera.h"

CAcquireItemText::CAcquireItemText(std::wstring text, COLORREF color)
{
	m_text = text;
	m_color = color;
	m_time = 1.5f;
}

CAcquireItemText::CAcquireItemText(const CAcquireItemText& other) : CObject(other)
{
}

CAcquireItemText* CAcquireItemText::Clone() const
{
	return new CAcquireItemText(*this);
}

void CAcquireItemText::update()
{
	m_time -= DT;

	if (m_time <= 0.0f)
		DeleteObj(this);
}

void CAcquireItemText::render(HDC _dc) const
{
	Vec2 pos = Mgr(CCamera)->GetRenderPos(m_vPos);

	static wchar_t buffer[128];

	SIZE s_out;
	GetTextExtentPoint32(_dc, m_text.c_str(), (int)m_text.size(), &s_out);

	Vec2 v_offset = Vec2::left * (s_out.cx * 0.5f);
	v_offset.y = sqrt(1.5f - m_time) * -64.0f;

	renderText(_dc, (COLORREF)0x00000000, pos + v_offset + Vec2::up * 2.0f, m_text);
	renderText(_dc, m_color, pos + v_offset, m_text);
}