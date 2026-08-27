#include "pch.h"
#include "CUI_InvenSlot.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CInvenSlotCart.h"
#include "CManagement.h"
#include "CInvenSlotBG.h"

CUI_InvenSlot::CUI_InvenSlot(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID)
	:CGameObject(pGraphicDev),m_eSlotNum(eID)
{
}

CUI_InvenSlot::CUI_InvenSlot(const CUI_InvenSlot& rhs)
	:CGameObject(rhs)
{
}

CUI_InvenSlot::~CUI_InvenSlot()
{
}

HRESULT CUI_InvenSlot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pItem = nullptr;

	//m_vPos = { 0,100,1 };
	m_vScale = { 250,400,1 };
	m_pTransformCom->Set_Scale(m_vScale);

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	return S_OK;
}

void CUI_InvenSlot::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_InvenSlot::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CInvenSlotCart* pItem = dynamic_cast<CInvenSlotCart*>(m_pItem);

	CInvenSlotBG* pBG = dynamic_cast<CInvenSlotBG*>(m_pBG);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (CheckCollisionUI(g_hWnd, vPos, m_vScale, m_pGraphicDev))
	{
		if (pItem)
		{
			pItem->SetTurn(true);
		}
		if (m_pBG)
		{
			pBG->SetMouseHover(true);
		}
		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{
			pBG->SetSelected();
		}
	}
	else
	{
		if (pItem)
		{
			pItem->SetTurn(false);
		}
		if (pBG)
		{
			pBG->SetMouseHover(false);
		}
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_InvenSlot::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_InvenSlot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	TCHAR   szFileName[128] = L"";
	wsprintf(szFileName, L"InvenSlot%d", m_eSlotNum);

	if(CRenderer::GetInstance()->Find_RenderTarget(szFileName))
		m_pGraphicDev->SetTexture(0, CRenderer::GetInstance()->Find_RenderTarget(szFileName)->pRTTexture);
	m_pVIBufferCom->Render_Buffer();
}

CUI_InvenSlot* CUI_InvenSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID)
{
	CUI_InvenSlot* pObj = new CUI_InvenSlot(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_InvenSlot Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_InvenSlot::Free()
{
	CGameObject::Free();
}