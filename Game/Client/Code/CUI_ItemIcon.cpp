#include "pch.h"
#include "CUI_ItemIcon.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CDInputMgr.h"
#include "CCart.h"
#include "CManagement.h"


CUI_ItemIcon::CUI_ItemIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_ItemIcon::CUI_ItemIcon(const CUI_ItemIcon& rhs)
	:CGameObject(rhs)
{
}

CUI_ItemIcon::~CUI_ItemIcon()
{
}

HRESULT CUI_ItemIcon::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemIconTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemSlotChangeTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	
	m_eFirstSlot = ITEM_END;
	m_eSecondSlot = ITEM_END;
	

	return S_OK;


}

void CUI_ItemIcon::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_ItemIcon::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CCart* pCart = dynamic_cast<CCart*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"));
	m_eFirstSlot = pCart->GetFirstSlot();
	m_eSecondSlot = pCart->GetSecondSlot();
	m_bSlotChange = pCart->GetSlotChange();
	if (m_bSlotChange) {
		pCart->SetSlotChange(false);
		m_iSlotChangeAnim = 0;
	}

	if (m_bSlotChange || m_iSlotChangeAnim != 0) {
		m_iSlotChangeAnim += 1;
		if (m_iSlotChangeAnim % 20 == 0) {
			m_bSlotChange = false;
			m_iSlotChangeAnim = 0;
		}
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_ItemIcon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_ItemIcon::Render_GameObject()
{
	if (m_eFirstSlot < ITEM_END)
	{
		m_pTransformCom->Set_Pos({ -505.f, 300.f, 1.f });
		m_pTransformCom->Set_Scale({ 80.f,80.f,0.f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_eFirstSlot);
		m_pVIBufferCom->Render_Buffer();
	}
	if (m_eSecondSlot < ITEM_END)
	{
		m_pTransformCom->Set_Pos({ -585.f, 310.f, 1.f });
		m_pTransformCom->Set_Scale({ 60.f,60.f,0.f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_eSecondSlot);
		m_pVIBufferCom->Render_Buffer();
	}
	m_pTransformCom->Set_Pos({ -425.f, 310.f, 1.f });
	m_pTransformCom->Set_Scale({ 60.f,60.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom2->Set_Texture(m_iSlotChangeAnim / 5);
	m_pVIBufferCom->Render_Buffer();
}

CUI_ItemIcon* CUI_ItemIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_ItemIcon* pObj = new CUI_ItemIcon(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_ItemIcon::Free()
{
	CGameObject::Free();
}