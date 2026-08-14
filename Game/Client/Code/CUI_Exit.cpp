#include "pch.h"
#include "CUI_Exit.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"

CUI_Exit::CUI_Exit(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_Exit::CUI_Exit(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_Exit::~CUI_Exit()
{
}

HRESULT CUI_Exit::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ -640,360,1 });
	m_pTransformCom->Set_Scale({ 100,100,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterIcon"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CUI_Exit::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Exit::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Exit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CUI_Exit* CUI_Exit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Exit* pObj = new CUI_Exit(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Exit Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_Exit::Free()
{
	CGameObject::Free();
}
