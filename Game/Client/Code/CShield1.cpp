#include "pch.h"
#include "CShield1.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CMissileTarget.h"

CShield1::CShield1(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CShield1::CShield1(const CGameObject& rhs) : CGameObject(rhs)
{
}

CShield1::~CShield1()
{
}

HRESULT CShield1::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	m_pTransformCom->Set_Scale({ 9.5f, 5.5f, 9.5f });

	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	//pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_NonAlpha"));
	//pComponent->Set_Owner(this);

	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	//m_fTimer = 0;
	//m_bCurState = false;

	return S_OK;
}

_int CShield1::Update_GameObject(const _float& fDeltaTime)
{
	// CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr)
	{
		m_pTransformCom->Set_Pos({ 0.f, -0.5f, 0.f });
	}

	//CMissileTarget* pTarget4 = dynamic_cast<CMissileTarget*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget4"));


	//if (pTarget4 != nullptr && pTarget4->GetBubbleUI())
	//	m_bCurState = true;

	//else
	//	m_bCurState = false;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CShield1::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CShield1::Render_GameObject()
{
	// 	if (m_bCurState == true)
	// 	{
	// m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pTextureCom->Set_Texture(0);
	// m_pBufferCom->Render_Buffer();
	// 	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	 m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	 m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CShield1* CShield1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShield1* pObj = new CShield1(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CShield1 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CShield1::Free()
{
	CGameObject::Free();
}
