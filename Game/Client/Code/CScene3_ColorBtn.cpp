#include "pch.h"
#include "CScene3_ColorBtn.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

#include "CScene3_ColorSetBG.h"
#include "CScene3_ColorSet.h"

CScene3_ColorBtn::CScene3_ColorBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_ColorBtn::CScene3_ColorBtn(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_ColorBtn::~CScene3_ColorBtn()
{
}

HRESULT CScene3_ColorBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -450,-47,10 };
	m_vScale = { 140, 84, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_ColorBtn"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

void CScene3_ColorBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}
	


_int CScene3_ColorBtn::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);
	CScene3_ColorSetBG* pSetBG = static_cast<CScene3_ColorSetBG*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSetBG"));
	CScene3_ColorSet* pRed = static_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet1"));
	CScene3_ColorSet* pGreen = static_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet2"));

	


	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
	{

		m_fFrame = 1;

		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{

			if (pSetBG->Get_Show() == false)
				pSetBG->Set_Show(true);
			else
				pSetBG->Set_Show(false);


			if (pRed->Get_Show1() == false)
				pRed->Set_Show1(true);
			else
				pRed->Set_Show1(false);


			if (pGreen->Get_Show1() == false)
				pGreen->Set_Show1(true);
			else
				pGreen->Set_Show1(false);
		}
	

	}

	else
	{
	
		m_fFrame = 0;
		
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_ColorBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_ColorBtn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene3_ColorBtn* CScene3_ColorBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_ColorBtn* pObj = new CScene3_ColorBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_ColorBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_ColorBtn::Free()
{
	CGameObject::Free();
}
