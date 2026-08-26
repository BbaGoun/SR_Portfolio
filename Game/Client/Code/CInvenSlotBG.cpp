#include "pch.h"
#include "CInvenSlotBG.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CInvenSlotBG::CInvenSlotBG(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID)
	:CGameObject(pGraphicDev), m_eSlotNum(eID)
{
}

CInvenSlotBG::CInvenSlotBG(const CInvenSlotBG& rhs)
	:CGameObject(rhs)
{
}

CInvenSlotBG::~CInvenSlotBG()
{
}

HRESULT CInvenSlotBG::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Pos({ 0,0,990 });
	m_pTransformCom->Set_Scale({ WINCX,WINCY,1 });

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_InvenWindow"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_bMouseHover	= false;
	m_fSelected		= false;
	return S_OK;
}

void CInvenSlotBG::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CInvenSlotBG::Update_GameObject(const _float& fDeltaTime)
{

	TCHAR   szFileName[128] = L"";
	wsprintf(szFileName, L"InvenSlot%d", m_eSlotNum);
	CRenderer::GetInstance()->Add_RenderTargetGroup(szFileName, this);
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CInvenSlotBG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CInvenSlotBG::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	if (m_fSelected == true)
	{
		m_fFrame = 2;
		if (m_bMouseHover == true) m_fFrame += 1;
	}
	else
	{
		m_fFrame = 0;
		if (m_bMouseHover == true) m_fFrame += 1;
	}
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}

void CInvenSlotBG::SetSelected()
{
	if (m_fSelected == true)
		m_fSelected = false;
	else
		m_fSelected = true;
}

CInvenSlotBG* CInvenSlotBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID)
{
	CInvenSlotBG* pObj = new CInvenSlotBG(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CInvenSlotBG Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CInvenSlotBG::Free()
{
	CGameObject::Free();
}