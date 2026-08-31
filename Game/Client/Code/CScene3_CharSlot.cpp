#include "pch.h"
#include "CScene3_CharSlot.h"
#include "CScene3_Char.h"
#include "CScene3_CharBG.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

#include "CManagement.h"


CScene3_CharSlot::CScene3_CharSlot(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
	:CGameObject(pGraphicDev), m_eSlotNum(eID)
{
}

CScene3_CharSlot::CScene3_CharSlot(const CScene3_CharSlot& rhs)
	:CGameObject(rhs)
{
}

CScene3_CharSlot::~CScene3_CharSlot()
{
}

HRESULT CScene3_CharSlot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pChar = nullptr;

	//m_vPos = { 0,100,1 };
	m_vScale = { 100,150,1 };
	m_pTransformCom->Set_Scale(m_vScale);

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	


	Set_Show(false);
	
	return S_OK;
}


void CScene3_CharSlot::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CScene3_CharSlot::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CScene3_Char* pChar = dynamic_cast<CScene3_Char*>(m_pChar);

	CScene3_CharBG* pBG = dynamic_cast<CScene3_CharBG*>(m_pBG);

	
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (CheckCollisionUI(g_hWnd, vPos, m_vScale, m_pGraphicDev))
	{
		if (pChar)
		{
			pChar->SetTurn(true);
		}
		if (m_pBG)
		{
			pBG->SetMouseHover(true);
		}
		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{
			pBG->SetSelected(true);

		}
	}
	else
	{
		if (pChar)
		{
			pChar->SetTurn(false);
		}
		if (pBG)
		{
			pBG->SetMouseHover(false);
		}
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_CharSlot::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_CharSlot::Render_GameObject()
{
	if (m_bShow == false)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	TCHAR   szFileName[128] = L"";
	wsprintf(szFileName, L"CharSlot%d", m_eSlotNum);
	
	if (CRenderer::GetInstance()->Find_RenderTarget(szFileName))
		m_pGraphicDev->SetTexture(0, CRenderer::GetInstance()->Find_RenderTarget(szFileName)->pRTTexture);
	m_pVIBufferCom->Render_Buffer();



	
}

CScene3_CharSlot* CScene3_CharSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
{
	CScene3_CharSlot* pObj = new CScene3_CharSlot(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_CharSlot Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene3_CharSlot::Free()
{
	CGameObject::Free();
	
}