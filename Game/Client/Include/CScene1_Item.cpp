#include "pch.h"
#include "CScene1_Item.h"

#include "CScene.h"
#include "CMenu_Item.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

CScene1_Item::CScene1_Item(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene1_Item::CScene1_Item(const CScene1_Item& rhs) : CGameObject(rhs)
{
}

CScene1_Item::~CScene1_Item()
{
}

HRESULT CScene1_Item::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_vPos = { 100,0,10 };
	m_vScale = { 78, 88, 0 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene1_Item"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	m_fFrame = 0;

	return S_OK;


}

void CScene1_Item::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
		{
		m_fFrame = 1;

		}
		
	else
		{
		m_fFrame = 0;
		}
	
}

_int CScene1_Item::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);



	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene1_Item::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene1_Item::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}

HRESULT CScene1_Item::Set_ClickIcon(const _float& fDeltaTime)
{
	
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
		if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB))
		{
			Engine::CScene* pStage = CMenu_Item::Create(m_pGraphicDev);

			if (nullptr == pStage)
				return E_FAIL;

			if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
			{
				MSG_BOX("CMenu_Item Create Failed");
				return -1;
			}
		}
			
}

	


CScene1_Item* CScene1_Item::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CScene1_Item* pObj = new CScene1_Item(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene1_Item Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene1_Item::Free()
{
	CGameObject::Free();
}
