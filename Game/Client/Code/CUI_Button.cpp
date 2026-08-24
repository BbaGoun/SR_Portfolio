#include "pch.h"
#include "CUI_Button.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CUI_Button::CUI_Button(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_Button::CUI_Button(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_Button::~CUI_Button()
{
}

HRESULT CUI_Button::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -549,-307.5,1 };
	m_vScale = { 182, 52, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ButtonEX"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

void CUI_Button::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame += 10 * fFixedDeltaTime;
		if (m_fFrame > 4)
			m_fFrame = 1;
		if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
			DestroyWindow(g_hWnd);
	}
	else
	{
		m_fFrame = 0;
	}
}

_int CUI_Button::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Button::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Button::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CUI_Button* CUI_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Button* pObj = new CUI_Button(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Button Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_Button::Free()
{
	CGameObject::Free();
}
