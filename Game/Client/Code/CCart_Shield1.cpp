#include "pch.h"
#include "CCart_Shield1.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CCartBody.h"

CCart_Shield1::CCart_Shield1(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CCart_Shield1::CCart_Shield1(const CGameObject& rhs) : CGameObject(rhs)
{
}

CCart_Shield1::~CCart_Shield1()
{
}

HRESULT CCart_Shield1::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0;
	m_bCurState = false;

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	m_pTransformCom->Set_Scale({ 10.5f, 03.4f, 10.5f });

	pComponent = m_pBufferCom = dynamic_cast<CHalfSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HalfSphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	//pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_NonAlpha"));
	//pComponent->Set_Owner(this);

	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CCart_Shield1::Update_GameObject(const _float& fDeltaTime)
{
	// CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr)
	{
		m_pTransformCom->Set_Pos({ 0.f, -1.5f, 0.f });
	}

	CCartBody* pCartBody = dynamic_cast<CCartBody*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody"));


	if (pCartBody != nullptr && pCartBody->GetShieldHit() == false && pCartBody->GetShieldActive() == true)
		m_bCurState = true;

	else
		m_bCurState = false;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCart_Shield1::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCart_Shield1::Render_GameObject()
{
	// 	if (m_bCurState == true)
	// 	{
	// m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pTextureCom->Set_Texture(0);
	// m_pBufferCom->Render_Buffer();
	// 	}

	if (m_bCurState == true)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		//m_pTextureCom->Set_Texture(0);
		m_pGraphicDev->SetTexture(0, nullptr);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

CCart_Shield1* CCart_Shield1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCart_Shield1* pObj = new CCart_Shield1(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCart_Shield1 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CCart_Shield1::Free()
{
	CGameObject::Free();
}
