#include "pch.h"
#include "CShield2.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CMissileTarget.h"

CShield2::CShield2(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CShield2::CShield2(const CGameObject& rhs) : CGameObject(rhs)
{
}

CShield2::~CShield2()
{
}

HRESULT CShield2::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0;
	m_bCurState = false;

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	m_pTransformCom->Set_Scale({ 12.f, 12.0f, 12.0f });

	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	//pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_NonAlpha"));
	//pComponent->Set_Owner(this);

	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CShield2::Update_GameObject(const _float& fDeltaTime)
{
	// CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr)
	{
		m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });
	}

	CMissileTarget* pTarget = dynamic_cast<CMissileTarget*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget"));


	if (pTarget != nullptr && pTarget->GetShieldHit() == true && pTarget->GetShieldActive() == true)
		m_bCurState = true;

	else
		m_bCurState = false;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CShield2::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CShield2::Render_GameObject()
{
	if (m_bCurState == true)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		// m_pTextureCom->Set_Texture(0);

		m_pGraphicDev->SetTexture(0, nullptr);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
 	}
}

CShield2* CShield2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShield2* pObj = new CShield2(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CShield2 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CShield2::Free()
{
	CGameObject::Free();
}
