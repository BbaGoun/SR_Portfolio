#include "pch.h"
#include "CUI_Laps.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CUI_Laps::CUI_Laps(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_Laps::CUI_Laps(const CUI_Laps& rhs)
	:CGameObject(rhs)
{
}

CUI_Laps::~CUI_Laps()
{
}

HRESULT CUI_Laps::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_NumberTexture"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CUI_Laps::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_Laps::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Laps::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Laps::Render_GameObject()
{
	m_pTransformCom->Set_Scale({ 80.f,48.f, 1.f });
	m_pTransformCom->Set_Pos({ 445.f, 250.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(12);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Scale({ 80.f,112.f, 1.f });
	m_pTransformCom->Set_Pos({ 530.f, 265.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iCurLap + 1);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Scale({ 40.f,56.f,1.f });
	m_pTransformCom->Set_Pos({ 565.f, 250.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(10);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 590.f, 250.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iMaxLap);
	m_pVIBufferCom->Render_Buffer();
}

CUI_Laps* CUI_Laps::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_Laps* pObj = new CUI_Laps(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Laps Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_Laps::Free()
{
	CGameObject::Free();
}