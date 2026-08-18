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
	virtual			void		Render_GameObject() override {};


public:
	static CCart*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void			KeyInput(const _float& fDeltaTime);
	void			UpdateDrift();
	void			UpdateBoost();
	bool			GetBoost() { return m_bBoost; }
	void			SetBoost(bool bBoost) { m_bBoost = bBoost; }

	bool			GetDrift() { return m_bDrift; }
	void			SetDrift(bool bDrift) { m_bDrift = bDrift; }

	bool			GetRainbowUI() { return m_bRainbowUI; }
	void			SetRainbowUI(bool bRainbowState) { m_bRainbowUI = bRainbowState; }

	void			SetBanana(bool bBanana) { m_bBanana = bBanana; }
	bool			GetBanana() { return m_bBanana; }

	void			CreateRainbowObject();
	void			CreateBananaObject();

	float			GetCurGage() { return m_fCurGage; }
	float			GetGainGage() { return m_fGainGage; }
	
	void			SetGainGage(float fGage) { m_fGainGage = fGage; }

	void			AdjustPosY_Slope(_vec3 pos);
	void			UpdateGravity();

	void			OutputCarState();

private:
	_float		m_fMaxSpeed;
	bool		m_bDrift;
	bool		m_bBoost;
	bool		m_bBanana;
	bool		m_bRainbowUI;

	float		m_fNormalTurnAngle;
	float		m_fBoostTurnAngle;
	float		m_fDriftTurnAngle;

	float		m_fLookForceAngle;
	float		m_fBoostCal;

	float		m_fBananaTimer;
	_vec3		m_vBananaSpinStartLook;

	float		m_fCurGage;
	float		m_fGainGage;

	float		m_fBoostItemCnt;

	CART_STATE	m_eCartState;
	_vec3		m_vTerrainNormal;


protected:
	virtual		void		Free() override;
};

