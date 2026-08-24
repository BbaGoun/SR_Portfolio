#include "pch.h"
#include "CScene2_ClockTower.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CScene2_ClockTower::CScene2_ClockTower(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene2_ClockTower::CScene2_ClockTower(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene2_ClockTower::~CScene2_ClockTower()
{
}

HRESULT CScene2_ClockTower::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_vPos = { 50,80,10 };
	m_vScale = { 150, 170, 1 };
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene2_ClockTower"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}



void CScene2_ClockTower::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 3;

	}

	else
	{
		m_fFrame = 2;
	}
}

_int CScene2_ClockTower::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene2_ClockTower::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene2_ClockTower::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CScene2_ClockTower* CScene2_ClockTower::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene2_ClockTower* pObj = new CScene2_ClockTower(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene2_ClockTower Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene2_ClockTower::Free()
{
	CGameObject::Free();
}
