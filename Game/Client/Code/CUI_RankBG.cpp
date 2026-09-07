#include "pch.h"
#include "CUI_RankBG.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CUI_RankBG::CUI_RankBG(LPDIRECT3DDEVICE9 pGraphicDev, RANK_ROW_OWNER eOwner, RANK_MARK_COLOR eColor) 
	: CGameObject(pGraphicDev)
	,m_eOwner(eOwner)
	,m_eColor(eColor)
{
}

CUI_RankBG::CUI_RankBG(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_RankBG::~CUI_RankBG()
{
}

HRESULT CUI_RankBG::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 200, 34, 1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RankRow"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CUI_RankBG::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_RankBG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_RankBG::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_eColor + m_eOwner * MARK_END);//

	m_pBufferCom->Render_Buffer();
}

CUI_RankBG* CUI_RankBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, RANK_ROW_OWNER eOwner, RANK_MARK_COLOR eColor)
{
	CUI_RankBG* pObj = new CUI_RankBG(pGraphicDev, eOwner, eColor);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_RankBG Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_RankBG::Free()
{
	CGameObject::Free();
}
