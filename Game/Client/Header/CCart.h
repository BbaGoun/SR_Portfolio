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
	void			UpdateBoost(const _float& fDeltaTime);
	void			UpdateThunder();

	bool			GetBoost()							{ if (m_eBoostState > 0)return true; else return false; }
	void			SetBoost(BOOST_STATE eID)			{ m_eBoostState = eID; }

	bool			GetDrift()							{ return m_bDrift; }
	void			SetDrift(bool bDrift)				{ m_bDrift = bDrift; }

	bool			GetRainbowUI()						{ return m_bRainbowUI; }
	void			SetRainbowUI(bool bRainbowState)	{ m_bRainbowUI = bRainbowState; }

	void			SetBanana(bool bBanana)				{ m_bBanana = bBanana; }
	bool			GetBanana()							{ return m_bBanana; }

	float			GetCurGage()						{ return m_fCurGage; }

	float			GetGainGage()						{ return m_fGainGage; }
	void			SetGainGage(float fGage)			{ m_fGainGage = fGage; }

	void			AdjustPosY_Slope(_vec3 pos);
	void			UpdateGravity();

	void			OutputCarState();

	void			CreateRainbowObject();
	void			CreateBananaObject();
	void			CreateThunderCloudObject();
	void			CreateMissileObject();					
	void			CreateTargetAimObject();				

private:
	_float		m_fMaxSpeed;
	bool		m_bDrift;
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

	bool		m_bThunder;

	bool		m_bShortBoosterOnOff;
	float		m_fShortBoosterTimer;

	CART_STATE	m_eCartState;
	BOOST_STATE	m_eBoostState;

	_vec3		m_vTerrainNormal;



protected:
	virtual		void		Free() override;
};

