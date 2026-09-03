#include "pch.h"
#include "CBoostWind.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CBoostWind::CBoostWind(LPDIRECT3DDEVICE9 pGraphicDev, BOOSTER_TYPE eID)
	:CGameObject(pGraphicDev), m_eBoosterID(eID)
{
}

CBoostWind::CBoostWind(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBoostWind::~CBoostWind()
{
}

HRESULT CBoostWind::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 16,16,16 });
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	D3DXQUATERNION q;
	switch (m_eBoosterID)
	{
	case Engine::WIND_L1:
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(-60), 0, 0.f);
		m_pTransformCom->Set_Quaternion(&q);
		m_pTransformCom->Set_Pos({ -4,2,6 });
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterWindL"));
		if (nullptr == pComponent)
			return E_FAIL;
		break;
	case Engine::WIND_L2:
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(-30), 0, 0.f);
		m_pTransformCom->Set_Quaternion(&q);
		m_pTransformCom->Set_Pos({ -6,2,6 });
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterWindL"));
		if (nullptr == pComponent)
			return E_FAIL;
		break;
	case Engine::WIND_R1:
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(60), 0, 0.f);
		m_pTransformCom->Set_Quaternion(&q);
		m_pTransformCom->Set_Pos({ 4,2,6 });
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterWindR"));
		if (nullptr == pComponent)
			return E_FAIL;
		break;
	case Engine::WIND_R2:
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(30), 0, 0.f);
		m_pTransformCom->Set_Quaternion(&q);
		m_pTransformCom->Set_Pos({ 6,2,6 });
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterWindR"));
		if (nullptr == pComponent)
			return E_FAIL;
		break;
	default:
		break;
	}

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CBoostWind::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CBoostWind::Update_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

		m_fFrame += 45.f * fDeltaTime;
		if (m_fFrame > 2.f)
			m_fFrame = 0;

		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CBoostWind::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBoostWind::Render_GameObject()
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
	{		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		m_pBufferCom->Render_Buffer();
	}
}

CBoostWind* CBoostWind::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOOSTER_TYPE eID)
{
	CBoostWind* pObj = new CBoostWind(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBoostWind Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBoostWind::Free()
{
	CGameObject::Free();
}
