#include "pch.h"
#include "CUI_BoosterBG.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"

CUI_BoosterBG::CUI_BoosterBG(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_BoosterBG::CUI_BoosterBG(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_BoosterBG::~CUI_BoosterBG()
{
}

HRESULT CUI_BoosterBG::Ready_GameObject()
{
	m_fSizeX = 209.f;

	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,-250,2 });
	m_pTransformCom->Set_Scale({ m_fSizeX,14,1 });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterBG"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });


	return S_OK;
}

_int CUI_BoosterBG::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_BoosterBG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_BoosterBG::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CUI_BoosterBG* CUI_BoosterBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_BoosterBG* pObj = new CUI_BoosterBG(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_BoosterBG Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_BoosterBG::Free()
{
	CGameObject::Free();
}
