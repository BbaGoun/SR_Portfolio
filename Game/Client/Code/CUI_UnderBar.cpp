#include "pch.h"
#include "CUI_UnderBar.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CUI_UnderBar::CUI_UnderBar(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_UnderBar::CUI_UnderBar(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_UnderBar::~CUI_UnderBar()
{
}

HRESULT CUI_UnderBar::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0,-342.5,10 });
	m_pTransformCom->Set_Scale({ 1280, 45, 1 });


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UI_UnderBar"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });


	m_vPos = { -575,-338, 10 };
	m_vScale = { 120, 45, 1 };
	m_fFrame = 0;

	return S_OK;
}

void CUI_UnderBar::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 1;
		//if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
			
	}

	else
	{
		m_fFrame = 0;
	}
}

_int CUI_UnderBar::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_UnderBar::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_UnderBar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CUI_UnderBar* CUI_UnderBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_UnderBar* pObj = new CUI_UnderBar(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_UnderBar Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_UnderBar::Free()
{
	CGameObject::Free();
}
