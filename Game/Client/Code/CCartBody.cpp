#include "pch.h"
#include "CCartBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CLand3.h"
#include "CManagement.h"
#include "CItemBox.h"
#include "CCollisionMgr.h"
#include "CMissileTarget.h"
#include "CCollisionStarEffect.h"
#include <SoundMgr.h>

CCartBody::CCartBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCartBody::CCartBody(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBody::~CCartBody()
{
}

HRESULT CCartBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_bBananaSpinState		= false;
	m_bThunderSpinState		= false;
	m_bThunderTimerOnOff	= false;

	m_fScale				= 1.f;
	m_fThunderTimer			= 0.f;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	//m_vColliderSize = { 2.5f,1.5f,5.f };
	//m_pColliderCom->Set_Extents(m_vColliderSize);
	
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_mapComponent.insert({ L"Com_Collider", pComponent });
	return S_OK;
}

void CCartBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	BananaSpin(fFixedDeltaTime); 
	ThunderSpin(fFixedDeltaTime);
	ThunderTimerUpdate(fFixedDeltaTime);

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(m_vRotation.y), D3DXToRadian(m_vRotation.x), D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
	
	//m_pTransformCom->Set_Scale({ m_fScale,m_fScale,m_fScale });
	//m_pColliderCom->Set_Extents(m_vColliderSize *m_fScale);
	//m_pTransformCom->Set_Pos({ 0,0.1f,0 });
}

_int CCartBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);	
}

void CCartBody::Render_GameObject()
{
	int a;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
}

void CCartBody::CollisionEnter(CCollider* pOtherCollider)
{
	const _tchar* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcsncmp(wOtherTag, L"Obj_CollisionBox", 16) == 0)
	{
		SoundMgr::GetInstance().PlaySound(L"Effect/cart/crash.ogg", COLLISION_EFFECT, 0.4f);
		_vec3 vParentForce = m_pParent->Get_Force();
		float vParentSpeed = m_pParent->Get_Speed();
		// StarEffect
		if (D3DXVec3Length(&vParentForce) * vParentSpeed >= 30)
		{
			CCollisionStarEffect* pStarParticle = dynamic_cast<CCollisionStarEffect*>
				(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"CollisionStarEffect"));
			pStarParticle->ResetParticle();
		}
		// MTV 적용
		_vec3 MTV =  CCollisionMgr::GetInstance()->GetMTVCubevsCube(
			static_cast<CCube_Collider*>(pOtherCollider), m_pColliderCom);
		CCart* pCart = dynamic_cast<CCart*>(m_pParent);

		// 살짝 뒤로 튕기기
		_vec3 vNewForce = pCart->Get_Force();
		vNewForce = MTV * D3DXVec3Length(&vNewForce) * 1.5f;
		float fForceLength = D3DXVec3Length(&vNewForce);
		if (fForceLength >= 30)
			vNewForce = vNewForce * 30 / fForceLength;
	
		_vec3 vPos;
		pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);

		pCart->Add_Force(vNewForce);
		pCart->Get_Transform()->Set_Pos(vPos+MTV);
		pCart->SetGainGage(0.f);	// m_fGainGage = 0.f;
		pCart->SetDrift(false);		// m_bDrift = false;
	}

	//////////////////////////////////////////////////////////////////////////////////// 테스트용  Obj_MissileTarget
	if (wcsncmp(wOtherTag, L"Obj_MissileTarget", 17) == 0)
	{
		_vec3 MTV = CCollisionMgr::GetInstance()->GetMTVCubevsCube(
			static_cast<CCube_Collider*>(pOtherCollider), m_pColliderCom);

		CCart* pCart = dynamic_cast<CCart*>(m_pParent);

		_vec3 vPos;
		pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
		pCart->Get_Transform()->Set_Pos(vPos + MTV);
	}
}

void CCartBody::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	CCart* pCart = dynamic_cast<CCart*>(m_pParent);
	if (wcsncmp(wOtherTag, L"Rainbow_Cloud", 13) == 0)
	{
		if (pCart->GetRainbowUI() == false)
			pCart->SetRainbowUI(true);
	}
	if (wcsncmp(wOtherTag, L"Obj_Banana", 10) == 0)
	{
		if (pCart->GetBanana() == false)
		{
			SoundMgr::GetInstance().PlaySound(L"Effect/Item_banana/Bananatrapped.ogg", SOUND_BANANA, 0.4f);
			pCart->SetBanana(true);
			pCart->SetBoost(BOOST_STATE_NORMAL);
		}
	}
	if (wcsncmp(wOtherTag, L"Obj_ItemBox", 11) == 0)
	{
		CItemBox* pItemBox = dynamic_cast<CItemBox*>(pOtherCollider->Get_Owner());
		if (pItemBox->GetShow() == true)
		{
			SoundMgr::GetInstance().PlaySound(L"Effect/ItemGain/eaten.ogg", SOUND_ITEMGAIN, 0.4f);
			pCart->GainItem();
			pItemBox->SetShow(false);
		}
	}
}
void CCartBody::BananaSpin(const _float& fDeltaTime)
{
	CCart* pCart = dynamic_cast<CCart*>(m_pParent);
	bool bCartBananaSpin = pCart->GetBanana();
	if (bCartBananaSpin == false)
		return;
	if (m_bBananaSpinState == false && bCartBananaSpin == true)
	{
		_vec3 vCartForce = pCart->Get_Force();
		m_fSpinSpeed = pCart->Get_Speed() * D3DXVec3Length(&vCartForce);
		m_fSpinSpeed = m_fSpinSpeed / 50 + 2;
		if (m_fSpinSpeed <= 1)m_fSpinSpeed++;
	}
	m_fSpinSpeed *= 0.98;
	m_bBananaSpinState = true;
	m_vRotation.y += 300 * m_fSpinSpeed * fDeltaTime;
	if (m_vRotation.y > 1080 * m_fSpinSpeed)
	{
		m_vRotation.y = 0;
		pCart->SetBanana(false);
		m_bBananaSpinState = false;
	}
}

void CCartBody::ThunderSpin(const _float& fDeltaTime)
{
	if (m_bThunderSpinState == false)
		return;
	m_vRotation.y += 720 * fDeltaTime;
	if (m_vRotation.y > 720)
	{
		m_vRotation.y = 0;
		m_bThunderSpinState = false;
	}
}

void CCartBody::ThunderTimerUpdate(const _float& fDeltaTime)
{
	if (m_bThunderTimerOnOff == false)
	{
		if (m_fScale < 1.f)
			m_fScale += fDeltaTime * 0.3f;
		else
			m_fScale = 1.f;
		return;
	}

	m_fThunderTimer += fDeltaTime;

	if (m_fScale > 0.5)
		m_fScale -= fDeltaTime;
	else
		m_fScale = 0.5f;

	if (m_fThunderTimer > 5.f)
	{
		m_bThunderTimerOnOff = false;
		m_fThunderTimer = 0.f;
	}
}

CCartBody* CCartBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBody* pObj = new CCartBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCartBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBody::Free()
{
	CGameObject::Free();
}
