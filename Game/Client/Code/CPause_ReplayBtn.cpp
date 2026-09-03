#include "pch.h"
#include "CPause_ReplayBtn.h"

#include "CScene.h"
#include "CMenu_Item.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "SoundMgr.h"

#include "CRacingScene.h"

CPause_ReplayBtn::CPause_ReplayBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CPause_ReplayBtn::CPause_ReplayBtn(const CPause_ReplayBtn& rhs) : CGameObject(rhs)
{
}

CPause_ReplayBtn::~CPause_ReplayBtn()
{
}

HRESULT CPause_ReplayBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;


	m_vPos = { -110,20,5 };
	m_vScale = { 100, 100, 0 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Pause_ReplayBtn"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	m_fFrame = 0;
	//Set_Show(false);

	return S_OK;


}

void CPause_ReplayBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{



}

_int CPause_ReplayBtn::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;


	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);



			if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
			{
				m_fFrame = 1;
				if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
				{
					Engine::CScene* pStage = CRacingScene::Create(m_pGraphicDev, MAP_TEST);

					if (nullptr == pStage)
						return E_FAIL;

					CManagement::GetInstance()->Request_Scene(pStage);
				}
			}

			else
			{
				m_fFrame = 0;
			}



	
	

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CPause_ReplayBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return ;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CPause_ReplayBtn::Render_GameObject()
{
	if (m_bShow == false)
		return ;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}



CPause_ReplayBtn* CPause_ReplayBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CPause_ReplayBtn* pObj = new CPause_ReplayBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CPause_ReplayBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CPause_ReplayBtn::Free()
{
	CGameObject::Free();
}
