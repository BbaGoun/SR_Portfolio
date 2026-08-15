#include "pch.h"
#include "CUI_BoosterBar.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"

CUI_BoosterBar::CUI_BoosterBar(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_BoosterBar::CUI_BoosterBar(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_BoosterBar::~CUI_BoosterBar()
{
}

HRESULT CUI_BoosterBar::Ready_GameObject()
{
	m_fCurGage = 0.f;
	m_fGainGage = 0.f;
	m_fSizeX = 209.f;

	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,-250,1 });
	m_pTransformCom->Set_Scale({ m_fSizeX,14,1 });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterBar"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });


	return S_OK;
}

_int CUI_BoosterBar::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	m_fCurGage += 10 * fDeltaTime;
	if (m_fCurGage > 100)
		m_fCurGage = 0;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_BoosterBar::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_BoosterBar::Render_GameObject()
{
	m_pTransformCom->Set_Scale({ m_fCurGage * 0.01f * m_fSizeX, 14, 1 });
	m_pTransformCom->Set_Pos({ 0.5f * (-m_fSizeX + m_fCurGage * 0.01f * m_fSizeX), -250, 1 });

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CUI_BoosterBar* CUI_BoosterBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_BoosterBar* pObj = new CUI_BoosterBar(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_BoosterBar Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_BoosterBar::Free()
{
	CGameObject::Free();
}
