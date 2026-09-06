#include "pch.h"
#include "CUI_LeftSideMirror.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CInvenSlotCart.h"
#include "CManagement.h"
#include "CInvenSlotBG.h"

CUI_LeftSideMirror::CUI_LeftSideMirror(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_LeftSideMirror::CUI_LeftSideMirror(const CUI_LeftSideMirror& rhs)
	:CGameObject(rhs)
{
}

CUI_LeftSideMirror::~CUI_LeftSideMirror()
{
}

HRESULT CUI_LeftSideMirror::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_vPos = { 0,0,10 };
	m_vScale = { 256, 384,1 };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	return S_OK;
}

void CUI_LeftSideMirror::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_LeftSideMirror::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_LeftSideMirror::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_LeftSideMirror::Render_GameObject()
{
	if (m_bShow == false)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (CRenderer::GetInstance()->Find_RenderTarget(L"LeftMirror"))
		m_pGraphicDev->SetTexture(0, CRenderer::GetInstance()->Find_RenderTarget(L"LeftMirror")->pRTTexture);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	m_pVIBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

CUI_LeftSideMirror* CUI_LeftSideMirror::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_LeftSideMirror* pObj = new CUI_LeftSideMirror(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_LeftSideMirror Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_LeftSideMirror::Free()
{
	CGameObject::Free();
}