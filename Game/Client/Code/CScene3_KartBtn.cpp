#include "pch.h"
#include "CScene3_KartBtn.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

#include "CMenu_Set.h"
#include <CUI_InvenSlot.h>

CScene3_KartBtn::CScene3_KartBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_KartBtn::CScene3_KartBtn(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_KartBtn::~CScene3_KartBtn()
{
}

HRESULT CScene3_KartBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -450,42,10 };
	m_vScale = { 140, 84, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_KartBtn"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;




	return S_OK;
}



void CScene3_KartBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	
}

_int CScene3_KartBtn::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 1;

		CUI_InvenSlot* pSlot = static_cast<CUI_InvenSlot*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_InvenSlot"));
		CUI_InvenSlot* pSlot2 = static_cast<CUI_InvenSlot*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_InvenSlot2"));

		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{
			
			if (pSlot->Get_Show() == true)
				pSlot->Set_Show(false);
			else
				pSlot->Set_Show(true);

			if (pSlot2   ->Get_Show() == true)
				pSlot2->Set_Show(false);
			else
				pSlot2->Set_Show(true);
		}
	}

	else
	{
		m_fFrame = 0;
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_KartBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_KartBtn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene3_KartBtn* CScene3_KartBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_KartBtn* pObj = new CScene3_KartBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_KartBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;

	
}

void CScene3_KartBtn::Free()
{
	CGameObject::Free();
}
