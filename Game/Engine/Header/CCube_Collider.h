#pragma once
#include "CCollider.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCube_Collider : public CCollider
{
private:
	explicit CCube_Collider();
	explicit CCube_Collider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Collider(const CCollider& rhs);
	virtual ~CCube_Collider();

public:
	virtual _int Update_Component(const _float& fTimeDelta); 
	virtual void LateUpdate_Component();

public:
	HRESULT		Ready_CCube_Collider();
	static		CCube_Collider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void		SetCenter(_vec3 vCenter) { m_vCenter = vCenter; }
	_vec3		GetCenter() { return m_vCenter; }

	void		SetSize(_vec3 vSize) { m_vSize = vSize; }
	_vec3		GetSize() { return m_vSize; }

	virtual		CComponent* Clone();

private:
	_vec3		m_vCenter;
	_vec3		m_vSize;

private:
	virtual		void		Free();

};
END
