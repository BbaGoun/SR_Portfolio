#include "pch.h"
#include "CUI_RankName.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CUI_RankName::CUI_RankName(LPDIRECT3DDEVICE9 pGraphicDev, RANK_NAME eName) : CGameObject(pGraphicDev),m_eName(eName)
{
}

CUI_RankName::CUI_RankName(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_RankName::~CUI_RankName()
{
}

HRESULT CUI_RankName::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 107.2, 32, 1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RankName"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CUI_RankName::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_RankName::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_RankName::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_eName);

	m_pBufferCom->Render_Buffer();
}

CUI_RankName* CUI_RankName::Create(LPDIRECT3DDEVICE9 pGraphicDev, RANK_NAME eName)
{
	CUI_RankName* pObj = new CUI_RankName(pGraphicDev,eName);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_RankName Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_RankName::Free()
{
	CGameObject::Free();
}
