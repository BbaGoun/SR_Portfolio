#include "pch.h"
#include "CScene1_Item.h"

#include "CScene.h"
#include "CPause_MenuBtn.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

#include "CStartMenu.h"


CPause_MenuBtn::CPause_MenuBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CPause_MenuBtn::CPause_MenuBtn(const CPause_MenuBtn& rhs) : CGameObject(rhs)
{
}

CPause_MenuBtn::~CPause_MenuBtn()
{
}

HRESULT CPause_MenuBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;


	m_vPos = { 120,20,5 };
	m_vScale = { 100, 100, 0 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Pause_MenuBtn"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	m_fFrame = 0;
	//Set_Show(false);
	return S_OK;


}

void CPause_MenuBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{


}

_int CPause_MenuBtn::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);


	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
	{
		m_fFrame = 1;
		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{
			Engine::CScene* pStage = CStartMenu::Create(m_pGraphicDev);

			if (nullptr == pStage)
				return E_FAIL;

			CManagement::GetInstance()->Request_Scene(pStage);
		}
	}
	else
		m_fFrame = 0;


	

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CPause_MenuBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return ;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CPause_MenuBtn::Render_GameObject()
{
	if (m_bShow == false)
		return ;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}



CPause_MenuBtn* CPause_MenuBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CPause_MenuBtn* pObj = new CPause_MenuBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CPause_MenuBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CPause_MenuBtn::Free()
{
	CGameObject::Free();
}
