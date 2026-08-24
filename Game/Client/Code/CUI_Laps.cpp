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
	m_pTransformCom->Set_Scale({ 32.f,44.f,0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UI_LapsTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;


	m_fFrame_Cur = 0;
	m_fFrame_Entire = 0;

	return S_OK;


}

void CUI_Laps::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_Laps::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);



	return CGameObject::Update_GameObject(fDeltaTime);


}

void CUI_Laps::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Laps::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ 550.f, 250.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_Cur);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 480.f, 250.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_Entire);
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