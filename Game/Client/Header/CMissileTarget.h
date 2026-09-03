#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CCartBodyCol;
	class CCube_Collider;
}
class CWaterBombBubble;
class CEmp_Band;

class CMissileTarget : public CGameObject
{
private:
	explicit CMissileTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissileTarget() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

					void		KeyInput(const _float& fDeltaTime);
				
	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void	    TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};
	
					void		CreateShieldObject();
					void		CreateEmp_BandObject();
					void		DeleteEmp_BandObject();
					void		ClearEmpBand();

					bool		GetMissileHit() { return m_bMissileHit; }
					void		SetMissileHit(bool bMissileHit) { m_bMissileHit = bMissileHit; }

					bool		GetBubbleUI() { return m_bBubbleUI; }
					void		SetBubbleUI(bool bBubbleState) { m_bBubbleUI = bBubbleState; }

					bool		GetShieldHit() { return m_bShieldHit; }
					void		SetShieldHit(bool bShieldState) { m_bShieldHit = bShieldState; }

					bool		GetShieldActive() { return m_bShieldActive; }
					void		SetShieldActivet(bool bActiveState) { m_bShieldActive = bActiveState; }

					bool		GetUfoHit() { return m_bUfoHit; }
public:
	static CMissileTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CCartBodyCol* m_pBufferCom;
	Engine::CCube_Collider* m_pColliderCom;
	CWaterBombBubble* m_pBubble;
	CEmp_Band* m_pEmpBand;
	_float				m_fMaxSpeed;
	_float				m_fTimer;
	_float				m_fShieldTimer;
	_float				m_fUfoTimer;

	_int				m_iLast_KeyInput;
	_int				m_iAccumulate;

	_bool				m_bMissileHit;
	_bool				m_bWaterBombHit;
	_bool				m_bWaterFlyHit;
	_bool				m_bWaterBubble;
	_bool				m_bWaterFly;
	_bool				m_bBubbling;
	_bool				m_bBubbleUI;
	_bool				m_bShieldHit;
	_bool				m_bShieldTimer;
	_bool				m_bShieldActive;
	_bool				m_bUfoHit;
	_bool				m_EmpBandCreate;

protected:
	virtual		void		Free() override;
};
