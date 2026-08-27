#include "pch.h"
#include "CScene3_StartBtn.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

CScene3_StartBtn::CScene3_StartBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_StartBtn::CScene3_StartBtn(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_StartBtn::~CScene3_StartBtn()
{
}

HRESULT CScene3_StartBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -450,220,10 };
	m_vScale = { 140, 84, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_StartBtn"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

void CScene3_StartBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
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

_int CScene3_StartBtn::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale, m_pGraphicDev))
		if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
		{
			Engine::CScene* pStage = CCollisionTest::Create(m_pGraphicDev);

			if (nullptr == pStage)
				return E_FAIL;

			CManagement::GetInstance()->Request_Scene(pStage);
			
		}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_StartBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_StartBtn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene3_StartBtn* CScene3_StartBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_StartBtn* pObj = new CScene3_StartBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_StartBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_StartBtn::Free()
{
	CGameObject::Free();
}
