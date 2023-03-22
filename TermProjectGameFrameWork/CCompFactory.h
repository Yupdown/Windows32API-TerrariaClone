#pragma once

class CObject;
class CComponent;

class CCompFactory
{
private:
	CCompFactory() = delete;
	~CCompFactory() = delete;
public:
	static CComponent* CreateComponent(COMPONENT_TYPE _eCompType, CObject* const _pOwner, Vec2 _vScale = {}, Vec2 _vPos = {});
};

