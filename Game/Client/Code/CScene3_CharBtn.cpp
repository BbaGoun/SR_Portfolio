#include "pch.h"
#include "CScene3_CharBtn.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

#include "CScene3_Map_ForestValley.h"
#include "CScene3_CharSlot.h"

CScene3_CharBtn::CScene3_CharBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_CharBtn::CScene3_CharBtn(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_CharBtn::~CScene3_CharBtn()
{
}

HRESULT CScene3_CharBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -450,131,10 };
	m_vScale = { 140, 84, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_CharBtn"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}


void CScene3_CharBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CScene3_CharBtn::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);



	CScene3_CharSlot* pSlot = static_cast<CScene3_CharSlot*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"CharSlot1"));
	CScene3_CharSlot* pSlot2 = static_cast<CScene3_CharSlot*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"CharSlot2"));

	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
	{
		m_fFrame = 1;


		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{
			if (pSlot->Get_Show() == false)
				pSlot->Set_Show(true);
			else
				pSlot->Set_Show(false);


			if (pSlot2->Get_Show() == false)
				pSlot2->Set_Show(true);
			else
				pSlot2->Set_Show(false);
		}

	}


	else
	{
		m_fFrame = 0;

	}



	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_CharBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_CharBtn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene3_CharBtn* CScene3_CharBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_CharBtn* pObj = new CScene3_CharBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_CharBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_CharBtn::Free()
{
	CGameObject::Free();
}
