#include "pch.h"
#include "CUI_Minimap.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CUI_Minimap::CUI_Minimap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_Minimap::CUI_Minimap(const CUI_Minimap& rhs)
	:CGameObject(rhs)
{
}

CUI_Minimap::~CUI_Minimap()
{
}

HRESULT CUI_Minimap::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Pos({ 500,0,1 });
	m_pTransformCom->Set_Scale({ 250,300,1 });

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	return S_OK;
}

void CUI_Minimap::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_Minimap::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Minimap::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Minimap::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	if (CRenderer::GetInstance()->Find_RenderTarget(L"Minimap"))
		m_pGraphicDev->SetTexture(0, CRenderer::GetInstance()->Find_RenderTarget(L"Minimap")->pRTTexture);
	m_pVIBufferCom->Render_Buffer();
}

CUI_Minimap* CUI_Minimap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Minimap* pObj = new CUI_Minimap(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Minimap Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_Minimap::Free()
{
	CGameObject::Free();
}