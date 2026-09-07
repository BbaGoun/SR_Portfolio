#pragma once
#include "CCollider.h"
#include "DirectXCollision.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCube_Collider : public CCollider
{
private:
	explicit CCube_Collider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Collider(const CCollider& rhs);	// 원본
	// explicit CCube_Collider(const CCube_Collider& rhs);	// 테스트
	virtual ~CCube_Collider();

public:
	virtual void FixedUpdate_Component(const _float& fFixedDeltaTime) override;
	virtual _int Update_Component(const _float& fTimeDelta); 
	virtual void LateUpdate_Component(const _float& fTimeDelta) override;
	virtual void Render_Component(D3DXCOLOR color) override;

public:
	HRESULT		Ready_CCube_Collider();
	static		CCube_Collider* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	DirectX::BoundingOrientedBox&	Get_Info()							{ return m_tBoundingBox; }

	void							Set_Center(_vec3 vPos)				{ m_tBoundingBox.Center = ToXMFLOAT3(vPos); }
	void							Set_Orientation(D3DXQUATERNION q)	{ m_tBoundingBox.Orientation = ToXMFLOAT4(q); }
	void							Set_Extents(_vec3 vExtents)			{ m_tBoundingBox.Extents = ToXMFLOAT3(vExtents); }
	_vec3							Get_Extents()						{ return ToVec3(m_tBoundingBox.Extents); }

	virtual		CComponent* Clone();

private:
	DirectX::BoundingOrientedBox	m_tBoundingBox;

private:
	virtual		void		Free();

};
END
