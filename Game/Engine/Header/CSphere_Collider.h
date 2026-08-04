#pragma once
#include "CCollider.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CSphere_Collider : public CCollider
{
private:
	explicit CSphere_Collider();
	explicit CSphere_Collider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphere_Collider(const CCollider& rhs);
	virtual ~CSphere_Collider();

public:
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();

public:
	HRESULT		Ready_CSphere_Collider();
	static		CSphere_Collider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void		SetCenter(_vec3 vCenter) { m_vCenter = vCenter; }
	_vec3		GetCenter() { return m_vCenter; }

	void		SetRadius(float fRadius) { m_fRadius = fRadius; }
	float		GetRadius() { return m_fRadius; }

	virtual		CComponent* Clone();

private:
	_vec3		m_vCenter;
	float		m_fRadius;

private:
	virtual		void		Free();

};
END
