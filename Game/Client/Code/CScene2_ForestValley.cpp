#include "pch.h"
#include "CScene2_ForestValley.h"

#include "CScene.h"
#include "CMenu_set.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

CScene2_ForestValley::CScene2_ForestValley(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene2_ForestValley::CScene2_ForestValley(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene2_ForestValley::~CScene2_ForestValley()
{
}

HRESULT CScene2_ForestValley::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -120,80,10 };
	m_vScale = { 150, 170, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene2_ForestValley"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

void CScene2_ForestValley::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
	{
		m_fFrame = 1;
		
	}

	else
	{
		m_fFrame = 0;
	}
}

_int CScene2_ForestValley::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
		if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
		{
			Engine::CScene* pStage = CMenu_Set::Create(m_pGraphicDev);

			if (nullptr == pStage)
				return E_FAIL;

			CManagement::GetInstance()->Request_Scene(pStage);
		
		}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene2_ForestValley::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene2_ForestValley::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene2_ForestValley* CScene2_ForestValley::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene2_ForestValley* pObj = new CScene2_ForestValley(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene2_ForestValley Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene2_ForestValley::Free()
{
	CGameObject::Free();
}
