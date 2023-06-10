#include "pch.h"
#include "CAcquireItemText.h"
#include "TRItemStack.h"
#include "TRItem.h"
#include "CCamera.h"

CAcquireItemText::CAcquireItemText(TRItemStack item)
{
	m_item = item;
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
	if (m_item.GetStackSize() > 1)
		wsprintf(buffer, L"%s (%d)", m_item.GetItem()->GetName().c_str(), m_item.GetStackSize());
	else
		wsprintf(buffer, L"%s", m_item.GetItem()->GetName().c_str());

	SIZE s_out;
	GetTextExtentPoint32(_dc, buffer, lstrlen(buffer), &s_out);

	Vec2 v_offset = Vec2::left * s_out.cx / 2;
	v_offset.y = sqrt(1.5f - m_time) * -64.0f;

	renderText(_dc, 0x00000000, pos + v_offset + Vec2::up * 2.0f, buffer);
	renderText(_dc, 0x00FFFFFF, pos + v_offset, buffer);
}