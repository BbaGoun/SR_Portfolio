#include "pch.h"
#include "CScene3_CharBG.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CScene3_CharBG::CScene3_CharBG(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
	:CGameObject(pGraphicDev), m_eSlotNum(eID)
{
}

CScene3_CharBG::CScene3_CharBG(const CScene3_CharBG& rhs)
	:CGameObject(rhs)
{
}

CScene3_CharBG::~CScene3_CharBG()
{
}

HRESULT CScene3_CharBG::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Pos({ 0,0,990 });
	m_pTransformCom->Set_Scale({ WINCX,WINCY,1 });

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_CharBG"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_bMouseHover = false;
	m_bSelected = false;
	return S_OK;
}

void CScene3_CharBG::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CScene3_CharBG::Update_GameObject(const _float& fDeltaTime)
{

	TCHAR   szFileName[128] = L"";
	wsprintf(szFileName, L"CharSlot%d", m_eSlotNum);
	CRenderer::GetInstance()->Add_RenderTargetGroup(szFileName, this);
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_CharBG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_CharBG::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	if (m_bSelected == true)
	{
		m_fFrame = 2;

	}
	else
	{
		m_fFrame = 0;
		if (m_bMouseHover == true) m_fFrame += 1;
	}
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}

void CScene3_CharBG::SetSelected(bool bSelected)
{

	bSelected = m_bSelected;
	if (m_bSelected == true)
		m_bSelected = false;
	else
		m_bSelected = true;

}

CScene3_CharBG* CScene3_CharBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
{
	CScene3_CharBG* pObj = new CScene3_CharBG(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_CharBG Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene3_CharBG::Free()
{
	CGameObject::Free();
}