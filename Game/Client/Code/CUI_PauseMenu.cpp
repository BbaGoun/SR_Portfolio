#include "pch.h"
#include "CUI_PauseMenu.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

#include "CManagement.h"

#include <CPause_ReplayBtn.h>
#include <CPause_MenuBtn.h>

CUI_PauseMenu::CUI_PauseMenu(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_PauseMenu::CUI_PauseMenu(const CUI_PauseMenu& rhs)
	:CGameObject(rhs)
{
}

CUI_PauseMenu::~CUI_PauseMenu()
{
}

HRESULT CUI_PauseMenu::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	
	m_pTransformCom->Set_Scale({650,390,0});
	m_pTransformCom->Set_Pos({ 0,0,6 });

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UI_PauseMenu"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;


	//Set_Show(false);

	return S_OK;
}

void CUI_PauseMenu::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_PauseMenu::Update_GameObject(const _float& fDeltaTime)
{
	

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CPause_ReplayBtn* pBtn = dynamic_cast<CPause_ReplayBtn*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"Pause_ReplayBtn"));
	CPause_MenuBtn* pBtn2 = dynamic_cast<CPause_MenuBtn*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"Pause_MenuBtn"));
	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_P))
	{
		if (Get_Show() == false)
		{
			Set_Show(true);
			pBtn->Set_Show(true);
			pBtn2->Set_Show(true);
		}
		else
		{
			Set_Show(false);
			pBtn->Set_Show(false);
			pBtn2->Set_Show(false);
		}
		
		
	}


	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_PauseMenu::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_PauseMenu::Render_GameObject()
{
	if (m_bShow == false)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();
}

CUI_PauseMenu* CUI_PauseMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_PauseMenu* pObj = new CUI_PauseMenu(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_PauseMenu Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_PauseMenu::Free()
{
	CGameObject::Free();
}