#include "pch.h"
#include "CItemUI.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CDInputMgr.h"


CItemUI::CItemUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItemUI::CItemUI(const CItemUI& rhs)
	:CGameObject(rhs)
{
}

CItemUI::~CItemUI()
{
}

HRESULT CItemUI::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemUITexture"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	
	m_fFirstSlot = ITEM_END;
	m_fSecondSlot = ITEM_END;
	

	return S_OK;


}

void CItemUI::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CItemUI::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	
	;

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_I))

	{

		if (m_fFirstSlot == ITEM_END)
		{
			m_fFirstSlot = rand() % ITEM_END;
		}

		else if (m_fFirstSlot != ITEM_END)
		{
			m_fSecondSlot = rand() % ITEM_END;
		}

	
	}

	
	

	

		if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_Z))
		{
			m_fFirstSlot = m_fSecondSlot;

			m_fSecondSlot = ITEM_END;
		}
	






	return CGameObject::Update_GameObject(fDeltaTime);


}

void CItemUI::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CItemUI::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ -495, 300, 10 });
	m_pTransformCom->Set_Scale({ 60.f,60.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFirstSlot);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ -595, 315, 10 });
	m_pTransformCom->Set_Scale({ 35.f,35.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fSecondSlot);
	m_pVIBufferCom->Render_Buffer();


}

CItemUI* CItemUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CItemUI* pObj = new CItemUI(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CItemUI::Free()
{
}