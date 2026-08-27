#include "pch.h"
#include "CScene3_Map_ForestValley.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

CScene3_Map_ForestValley::CScene3_Map_ForestValley(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_Map_ForestValley::CScene3_Map_ForestValley(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_Map_ForestValley::~CScene3_Map_ForestValley()
{
}

HRESULT CScene3_Map_ForestValley::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_Map_ForestValley"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}


void CScene3_Map_ForestValley::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CScene3_Map_ForestValley::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_Map_ForestValley::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_Map_ForestValley::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ -250.f, -150.f, 10.f });
	m_pTransformCom->Set_Scale({ 200,160,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ -40.f, -150.f, 10.f });
	m_pTransformCom->Set_Scale({ 200,160,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(1);
	m_pBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 216.f, 30.f, 10.f });
	m_pTransformCom->Set_Scale({ 585,520,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();
}

CScene3_Map_ForestValley* CScene3_Map_ForestValley::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_Map_ForestValley* pObj = new CScene3_Map_ForestValley(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_Map_ForestValley Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_Map_ForestValley::Free()
{
	CGameObject::Free();
}
