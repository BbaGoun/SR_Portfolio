#include "pch.h"
#include "CUI_Menu.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CUI_Menu::CUI_Menu(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_Menu::CUI_Menu(const CUI_Menu& rhs) : CGameObject(rhs)
{
}

CUI_Menu::~CUI_Menu()
{
}

HRESULT CUI_Menu::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MenuBG"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;


}

_int CUI_Menu::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Menu::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Menu::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ 0.f, 0.f, 2.f });
	m_pTransformCom->Set_Scale({ 1.f,1.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();
}

	


CUI_Menu* CUI_Menu::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_Menu* pObj = new CUI_Menu(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("UI_Menu Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_Menu::Free()
{
	CGameObject::Free();
}
