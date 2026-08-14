#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCube_Collider;
	class CSphere_Collider;
}

class CCart :  public CGameObject
{
private:
	explicit CCart(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCart(const CGameObject& rhs);
	virtual ~CCart() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

public:
	static CCart*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void			KeyInput(const _float& fDeltaTime);
	void			UpdateDrift();
	void			UpdateBoost();
	bool			GetBoost() { return m_bBoost; }
	bool			GetDrift() { return m_bDrift; }
	bool			GetRainbowUI() { return m_bRainbowUI; }

	void			SetRainbowUI(bool bRainbowState) { m_bRainbowUI = bRainbowState; }

	void			CreateRainbowObject();
private:
	_float		m_fMaxSpeed;
	bool		m_bDrift;

	float		m_fNormalTurnAngle;
	float		m_fBoostTurnAngle;
	float		m_fDriftTurnAngle;

	float		m_fLookForceAngle;
	bool		m_bBoost;
	float		m_fBoostCal;

	float		m_bRainbowUI;
	int			m_iRainbowObjectCnt;

	Engine::CCube_Collider* m_pColliderCom;

protected:
	virtual		void		Free() override;
};

