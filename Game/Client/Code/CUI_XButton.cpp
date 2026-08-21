#include "pch.h"
#include "CUI_XButton.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CUI_XButton::CUI_XButton(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_XButton::CUI_XButton(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_XButton::~CUI_XButton()
{
}

HRESULT CUI_XButton::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { 580,320,10 };
	m_vScale = { 64, 50, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UI_XButton"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

void CUI_XButton::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 1;
		if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
			DestroyWindow(g_hWnd);
	}

	else
	{
		m_fFrame = 0;
	}
}

_int CUI_XButton::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_XButton::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_XButton::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CUI_XButton* CUI_XButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_XButton* pObj = new CUI_XButton(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CXButton Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_XButton::Free()
{
	CGameObject::Free();
}
