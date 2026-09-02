#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CCube_Collider;
}

class CUfo : public CGameObject
{
private:
	explicit CUfo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUfo() override;

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
	_float			m_fUfoBack;
	_float			m_fUfoFront;
	_float			m_fUfoRight;
	_float			m_fSpeed;
	_float			m_fAngle;
	// _float			m_fAngle;	// 풍선 최대치 크기 도달 후 회전 이거 말고 -> D3DXToRadian 이걸로?
	_bool			m_bSavePos;
	_bool			m_bFollowTag;

	_vec3			m_vSavePos;

public:
	static CUfo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
