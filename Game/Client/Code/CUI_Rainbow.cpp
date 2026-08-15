#include "pch.h"
#include "CUI_Rainbow.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"

CUI_Rainbow::CUI_Rainbow(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_Rainbow::CUI_Rainbow(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_Rainbow::~CUI_Rainbow()
{
}

HRESULT CUI_Rainbow::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0,0,1 });
	m_pTransformCom->Set_Scale({ 800,600,1 });


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_NonAlpha"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	m_fTimer = 0;
	m_bCurState = false;

	return S_OK;
}

_int CUI_Rainbow::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Cart", L"Com_Transform");
	CCart* pCart = dynamic_cast<CCart*>(pCom->Get_Owner());

	if (m_bCurState == false && pCart->GetRainbowUI())
		m_bCurState = true;

	if (m_bCurState == true)
		m_fTimer += fDeltaTime;

	if (m_fTimer > 4)
	{
		m_fTimer = 0;
		m_bCurState = false;
		pCart->SetRainbowUI(false);
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Rainbow::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Rainbow::Render_GameObject()
{
	if (m_bCurState == true)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(0);

		m_pBufferCom->Render_Buffer();
	}
}

CUI_Rainbow* CUI_Rainbow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Rainbow* pObj = new CUI_Rainbow(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Rainbow Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_Rainbow::Free()
{
	CGameObject::Free();
}
