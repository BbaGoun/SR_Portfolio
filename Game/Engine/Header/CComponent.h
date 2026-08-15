#pragma once

#include "CBase.h"
#include "Engine_Define.h"


BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int Update_Component(const _float& fDeltaTime) { return 0; }
	virtual void LateUpdate_Component(const _float& fDeltaTime) { }
	
public:
	void Set_Owner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
	CGameObject* Get_Owner() { return m_pOwner; }
	COMPONENTID Get_ID() { return m_eID; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bClone;
	CGameObject*				m_pOwner;
	COMPONENTID					m_eID;

public:
	virtual CComponent* Clone()	PURE;		// 프로토타입 디자인 패턴을 사용하는 예

protected:
	virtual void		Free();
};

END