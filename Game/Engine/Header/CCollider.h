#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollider :  public CComponent
{

protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	virtual _int Update_Component(const _float& fTimeDelta);	
	virtual void LateUpdate_Component();		
	virtual void Render_Component(D3DXCOLOR color) {}

public:
	HRESULT			Ready_CColliderCom();
	static			CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void			SetIsTrigger(bool bTrigger)				{ m_bIsTrigger = bTrigger; }
	bool			GetIsTrigger()							{ return m_bIsTrigger; }
	void			Set_Offset(_vec3 vOffset) { m_vOffset = vOffset; }
	_vec3			Get_Offset() { return m_vOffset; }
	void			Set_Active(bool _b) { m_bActive = _b; }
	bool			Get_Active() { return m_bActive; }

	COLLIDER_TYPE	GetColliderType()						{ return m_eColliderType; }

	virtual			CComponent* Clone(); //PURE로 설정

protected:
	bool				m_bIsTrigger;
	COLLIDER_TYPE		m_eColliderType;
	_vec3				m_vOffset = { 0,0,0 };
	bool				m_bActive = true;

protected:
	virtual		void		Free();
};

END
