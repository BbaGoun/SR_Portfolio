#include "pch.h"
#include "CUI_RankNumber.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CDInputMgr.h"
#include "CCart.h"
#include "CManagement.h"
#include "CTrackMgr.h"


CUI_RankNumber::CUI_RankNumber(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_RankNumber::CUI_RankNumber(const CUI_RankNumber& rhs)
	:CGameObject(rhs)
{
}

CUI_RankNumber::~CUI_RankNumber()
{
}

HRESULT CUI_RankNumber::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_NumberTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	return S_OK;
}

void CUI_RankNumber::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_RankNumber::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHAUI, this);

	m_iRank = CTrackMgr::GetInstance()->Get_Rank(m_pCart);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_RankNumber::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_RankNumber::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ -570, 120, 2 });
	m_pTransformCom->Set_Scale({ 80, 112, 0 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iRank + 1);
	m_pVIBufferCom->Render_Buffer();

	// 'st, nd, rd, th'
	m_pTransformCom->Set_Pos({ -510, 100, 2 });
	m_pTransformCom->Set_Scale({ 40, 56, 0 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(14 + m_iRank % 4);
	m_pVIBufferCom->Render_Buffer();
	
	// '/'
	m_pTransformCom->Set_Pos({ -475, 100, 2 });
	m_pTransformCom->Set_Scale({ 40, 56, 0 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(10);
	m_pVIBufferCom->Render_Buffer();

	// max
	m_pTransformCom->Set_Pos({ -450, 100, 2 });
	m_pTransformCom->Set_Scale({ 40, 56, 0 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(4);
	m_pVIBufferCom->Render_Buffer();
}

CUI_RankNumber* CUI_RankNumber::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_RankNumber* pObj = new CUI_RankNumber(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_RankNumber::Free()
{
	CGameObject::Free();
}