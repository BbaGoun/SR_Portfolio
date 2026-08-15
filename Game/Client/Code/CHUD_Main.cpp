#include "pch.h"
#include "CHUD_Main.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"

CHUD_Main::CHUD_Main(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CHUD_Main::CHUD_Main(const CGameObject& rhs) : CGameObject(rhs)
{
}

CHUD_Main::~CHUD_Main()
{
}

HRESULT CHUD_Main::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,-307.5,1 });

	m_pTransformCom->Set_Scale({ 282, 105, 1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HUDMain"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CHUD_Main::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CHUD_Main::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CHUD_Main::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CHUD_Main* CHUD_Main::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHUD_Main* pObj = new CHUD_Main(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CHUD_Main Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CHUD_Main::Free()
{
	CGameObject::Free();
}
