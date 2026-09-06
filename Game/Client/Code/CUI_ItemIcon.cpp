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
	
	m_eFirstSlot = ITEM_END;
	m_eSecondSlot = ITEM_END;
	

	return S_OK;
}

void CUI_ItemIcon::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_ItemIcon::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHAUI, this);

	if (m_pCart) {
		m_eFirstSlot = m_pCart->GetFirstSlot();
		m_eSecondSlot = m_pCart->GetSecondSlot();

	}
	else if (m_pCartBot) {
		m_eFirstSlot = m_pCartBot->GetFirstSlot();
		m_eSecondSlot = m_pCartBot->GetSecondSlot();
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
		m_pTransformCom->Set_Pos(m_vPosFirst);
		m_pTransformCom->Set_Scale(m_vScaleFirst);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_eFirstSlot);
		m_pVIBufferCom->Render_Buffer();
	}
	if (m_eSecondSlot < ITEM_END)
	{
		m_pTransformCom->Set_Pos(m_vPosSecond);
		m_pTransformCom->Set_Scale(m_vScaleSecond);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_eSecondSlot);
		m_pVIBufferCom->Render_Buffer();
	}
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