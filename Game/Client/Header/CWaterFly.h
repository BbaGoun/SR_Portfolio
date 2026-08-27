#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CCube_Collider;
}

class CWaterFly : public CGameObject
{
private:
	explicit CWaterFly(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterFly() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

private:
	Engine::CCube_Collider* m_pColliderCom;
	_float			m_fTimer;
	_float			m_fFlyBack;
	_float			m_fFlyFront;
	 _float			m_fSpeed;
	// _float			m_fAngle;	// 풍선 최대치 크기 도달 후 회전 이거 말고 -> D3DXToRadian 이걸로?
	_bool			m_bSavePos;

	_vec3			m_vSavePos;
	
public:
	static CWaterFly* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
