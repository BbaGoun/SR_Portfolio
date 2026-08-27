#pragma once
#include "CCollider.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CSphere_Collider : public CCollider
{
private:
	explicit CSphere_Collider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphere_Collider(const CCollider& rhs);
	virtual ~CSphere_Collider();

public:
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component(const _float& fTimeDelta) override;
	virtual void Render_Component(D3DXCOLOR color) override;

public:
	HRESULT		Ready_CSphere_Collider();
	static		CSphere_Collider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	DirectX::BoundingSphere& Get_Info()		{ return m_tBoundingSphere; }

	void		Set_Center(_vec3 vPos)		{ m_tBoundingSphere.Center = ToXMFLOAT3(vPos); }
	void		Set_Radius(float fRadius)	{ m_tBoundingSphere.Radius = max(0.01f, fRadius); }

	virtual		CComponent* Clone();

private:
	DirectX::BoundingSphere	m_tBoundingSphere;

private:
	virtual		void		Free();

};
END
