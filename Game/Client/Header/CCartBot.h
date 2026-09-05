#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
#include "CPlayerHead.h"

namespace Engine
{
	class CCube_Collider;
	class CSphere_Collider;
}

class CCartBot : public CGameObject
{
private:
	explicit CCartBot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCartBot(const CGameObject& rhs);
	virtual ~CCartBot() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override {};


public:
	static CCartBot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	// KeyInput
	void			KeyInput(const _float& fDeltaTime);

	// Update
	void			UpdateDrift(const _float fDeltaTime);
	void			UpdateBoost(const _float& fDeltaTime);
	void			UpdateThunder();
	void			UpdateGravity();
	void			UpdateMagnet(const _float& fDeltaTime);
	void			UpdateStartBoost();
	void			UpdateBlur(const _float& fDeltaTime);
	void			UpdateBubble(const _float& fDeltaTime);

	// Get, Set
	bool			GetBoost() { if (m_eBoostState > 0)return true; else return false; }
	void			SetBoost(BOOST_STATE eID) { m_eBoostState = eID; }

	bool			GetDrift() { return m_bDrift; }
	void			SetDrift(bool bDrift) { m_bDrift = bDrift; }

	bool			GetRainbowUI() { return m_bRainbowUI; }
	void			SetRainbowUI(bool bRainbowState) { m_bRainbowUI = bRainbowState; }

	// bool			GetBubbleUI()						{ return m_bBubbleUI; }
	// void			SetBubbleUI(bool bBubbleUI)			{ m_bBubbleUI = bBubbleUI; }

	void			SetBanana(bool bBanana) { m_bBanana = bBanana; }
	bool			GetBanana() { return m_bBanana; }

	float			GetCurGage() { return m_fCurGage; }

	float			GetGainGage() { return m_fGainGage; }
	void			SetGainGage(float fGage) { m_fGainGage = fGage; }

	ITEM_TYPE		GetFirstSlot() { return m_eFirstSlot; }
	void			SetFirstSlot(ITEM_TYPE eID) { m_eFirstSlot = eID; }

	ITEM_TYPE		GetSecondSlot() { return m_eSecondSlot; }
	void			SetSecondSlot(ITEM_TYPE eID) { m_eSecondSlot = eID; }


	bool			GetPlayingState() { return m_bPlaying; }

	// AdjustState
	void			AdjustPosY_Slope(_vec3 pos, const float fDeltaTime);

	// WallCollision
	void			CollisionWall();

	// CreateObject
	void			CreateRainbowObject();
	void			CreateBananaObject();
	void			CreateThunderCloudObject();
	void			CreateMissileObject(CGameObject* pTarget);
	void			CreateMagnetObject();
	void			CreateWaterBombObject();
	void			CreateWaterFlyObject();
	void			CreateTargetAimObject();
	void			CreateMissileAimObject();
	void			CreateMagnetAimObject();
	void			CreateShieldObject();

	// Item
	void			GainItem();
	void			GainBoost();
	void			UseItem();
	void			UseAimItem();
	void			UseMissileItem();
	void			UseMagnetItem();

	// OutputState
	void			OutputCarState();

	// SetPlayerHead
	void			SetPlayerHead(CGameObject* pPlayerHead) { m_pPlayerHead = static_cast<CPlayerHead*>(pPlayerHead); }

	// Wheel
	void			AddWheel();
	void			SetWheelForceLen();
	void			SetWheelDir();
	void			SetWheelTurn(WHEEL_TURN eTurn);

	// Shield
	void			SetShield1(CGameObject* pShield1) { m_pShield1 = pShield1; }
	void			SetShield2(CGameObject* pShield2) { m_pShield2 = pShield2; }

	CGameObject*	GetShield1() { return m_pShield1; }
	CGameObject*	GetShield2() { return m_pShield2; }

	//Missile
	void			SetMissileHit(bool bHit)	{ m_bMissileHit = bHit; }
	bool			GetMissileHit()				{ return m_bMissileHit; }

	//Bubble
	void			SetBubble(bool bBubble) { if (m_bBubble == false) m_bBubble = true; }
	bool			GetBubble() { return m_bBubble; }
	void			SetBubble(CGameObject* pBubble) { m_pBubble = pBubble; }

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

	CPlayerHead* m_pPlayerHead;
	bool			m_bUpKey;

	float			m_fLateralOffsetTarget = 0.f;
	float			m_fLateralOffset = 0.f;
	float			m_fOffsetTimer = 0.f;
	float			m_fOffsetTimerEnd = 2.f;

	vector<CGameObject*>	m_vecWheel;

	bool			m_bCollisionGround = false;
	bool			m_bCollisionWall = false;

	_float			m_fAimRotationZ;


	CGameObject*	m_pShield1 = nullptr;
	CGameObject*	m_pShield2 = nullptr;

	bool			m_bMissileHit = false;
	bool			m_bBubble = false;
	float			m_fBubbleTimer = 0.f;
	CGameObject*	m_pBubble = nullptr;

protected:
	virtual		void		Free() override;
};

