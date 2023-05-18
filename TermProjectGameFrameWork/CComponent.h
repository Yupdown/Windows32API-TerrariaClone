#pragma once

class CObject;

class CComponent
{
	friend class CCompFactory;
protected:
	CObject* m_pOwner = nullptr;
protected:
	CComponent() = default;
	CComponent(const CComponent&) = default;
public:
	void SetOwner(CObject* const _pOwner) { m_pOwner = _pOwner; }
	CObject* GetOwner()const { return m_pOwner; }
	virtual CComponent* Clone(CObject* const _pOwner)const = 0;
	virtual ~CComponent() = default;
	virtual void component_update() = 0;
	virtual void component_render(HDC _dc)const = 0;
};

