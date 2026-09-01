#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
#include "CPlayerHead.h"

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
	// KeyInput
	void			KeyInput(const _float& fDeltaTime);

	// Update
	void			UpdateDrift();
	void			UpdateBoost(const _float& fDeltaTime);
	void			UpdateThunder();
	void			UpdateGravity();
	void			UpdateMagnet(const _float& fDeltaTime);
	void			UpdateStartBoost();

	// Get, Set
	bool			GetBoost()							{ if (m_eBoostState > 0)return true; else return false; }
	void			SetBoost(BOOST_STATE eID)			{ m_eBoostState = eID; }

	bool			GetDrift()							{ return m_bDrift; }
	void			SetDrift(bool bDrift)				{ m_bDrift = bDrift; }

	bool			GetRainbowUI()						{ return m_bRainbowUI; }
	void			SetRainbowUI(bool bRainbowState)	{ m_bRainbowUI = bRainbowState; }

	// bool			GetBubbleUI()						{ return m_bBubbleUI; }
	// void			SetBubbleUI(bool bBubbleUI)			{ m_bBubbleUI = bBubbleUI; }

	void			SetBanana(bool bBanana)				{ m_bBanana = bBanana; }
	bool			GetBanana()							{ return m_bBanana; }

	float			GetCurGage()						{ return m_fCurGage; }

	float			GetGainGage()						{ return m_fGainGage; }
	void			SetGainGage(float fGage)			{ m_fGainGage = fGage; }

	ITEM_TYPE		GetFirstSlot()						{ return m_eFirstSlot; }
	void			SetFirstSlot(ITEM_TYPE eID)			{ m_eFirstSlot = eID; }

	ITEM_TYPE		GetSecondSlot()						{ return m_eSecondSlot; }
	void			SetSecondSlot(ITEM_TYPE eID)		{ m_eSecondSlot = eID; }


	bool			GetPlayingState()					{ return m_bPlaying; }

	// AdjustState
	void			AdjustPosY_Slope(_vec3 pos, const float fDeltaTime);

	// WallCollision
	void			CollisionWall();

	// CreateObject
	void			CreateRainbowObject();
	void			CreateBananaObject();
	void			CreateThunderCloudObject();
	void			CreateMissileObject();	
	void			CreateMagnetObject();
	void			CreateWaterBombObject();
	void			CreateWaterFlyObject();
	void			CreateTargetAimObject();	
	void			CreateMissileAimObject();
	void			CreateMagnetAimObject();
	void			CreateShieldObject_();
	void			CreateUfoObject();

	// Item
	void			GainItem();
	void			UseItem();
	void			UseAimItem();
	void			UseMissileItem();
	void			UseMagnetItem();

	// OutputState
	void			OutputCarState();

	// SetPlayerHead
	void			SetPlayerHead(CGameObject* pPlayerHead) { m_pPlayerHead = static_cast<CPlayerHead*>(pPlayerHead); }

private:
	_float			m_fMaxSpeed;
	bool			m_bDrift;
	bool			m_bBanana;
	bool			m_bRainbowUI;
	// _bool			m_bBubbleUI;
	_bool			m_bMagnet;
	_float			m_fMagnetTimer;

	_bool			m_bUseItem;

	float			m_fNormalTurnAngle;
	float			m_fBoostTurnAngle;
	float			m_fDriftTurnAngle;

	float			m_fLookForceAngle;
	float			m_fBoostCal;

	float			m_fBananaTimer;
	_vec3			m_vBananaSpinStartLook;

	float			m_fCurGage;
	float			m_fGainGage;

	float			m_fBoostItemCnt;

	bool			m_bThunder;

	bool			m_bShortBoosterTimerOnOff;
	float			m_fShortBoosterTimer;

	CART_STATE		m_eCartState;
	BOOST_STATE		m_eBoostState;

	_vec3			m_vTerrainNormal;

	float			m_fAirTime;
	D3DXQUATERNION	m_PreQuaternion;

	ITEM_TYPE		m_eFirstSlot;
	ITEM_TYPE		m_eSecondSlot;

	DIRECTION_TYPE	m_eDirection;
	int				m_iFlatFrameCnt;

	bool			m_bCanShortBoost;

	float			m_fPlayTimer;
	float			m_fPreTimer;
	bool			m_bPlaying;

	CPlayerHead*	m_pPlayerHead;

protected:
	virtual		void		Free() override;
};

