#include "pch.h"
#include "CScene3_CharBtn.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

CScene3_CharBtn::CScene3_CharBtn(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_CharBtn::CScene3_CharBtn(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_CharBtn::~CScene3_CharBtn()
{
}

HRESULT CScene3_CharBtn::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { -450,131,10 };
	m_vScale = { 140, 84, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_CharBtn"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

//HRESULT CScene3_KartBtn::Set_ClickIcon(const _float& fDeltaTime)
//{
//
//}

void CScene3_CharBtn::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 1;

	}

	else
	{
		m_fFrame = 0;
	}
}

_int CScene3_CharBtn::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_CharBtn::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_CharBtn::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene3_CharBtn* CScene3_CharBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_CharBtn* pObj = new CScene3_CharBtn(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_CharBtn Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_CharBtn::Free()
{
	CGameObject::Free();
}
