#include "pch.h"
#include "CGOMay.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CCameraMgr.h"

CGOMay::CGOMay(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CGOMay::CGOMay(const CGameObject& rhs) : CGameObject(rhs)
{
}

CGOMay::~CGOMay()
{
}

HRESULT CGOMay::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	//m_pTransformCom->m_qRotation = { sinf(D3DXToRadian(90) / (float)2), 0, 0, cosf(D3DXToRadian(90) / (float)2) };
	//m_pTransformCom->m_vScale = { 0.05f, 0.05f, 0.05f };

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CMay*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_May"));
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

_int CGOMay::Update_GameObject(const _float& fTimeDelta)
{
	return CGameObject::Update_GameObject(fTimeDelta);
}

void CGOMay::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Follow(fTimeDelta);
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGOMay::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	//for (int i = 0; i < CAM_GLOBAL; ++i)
	//{
	//	switch (i) {
	//	case 0:
	//		m_pGraphicDev->SetViewport(&g_LeftView);
	//		break;
	//	case 1:
	//		m_pGraphicDev->SetViewport(&g_RightView);
	//		break;
	//	}
	//	CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo();
	//	m_pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
	//	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
	//	m_pBufferCom->Render_Buffer();
	//}
}

void CGOMay::Follow(const _float& fTimeDelta)
{
	_vec3 playerPos;
	static_cast<CTransform*>(
		CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject", L"Obj_Player", L"Com_Transform")
		)->Get_Info(INFO_POS, &playerPos);

	m_pTransformCom->FollowObj(&playerPos, 10.f, fTimeDelta);
}

CGOMay* CGOMay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGOMay* pGOMay = new CGOMay(pGraphicDev);

	if (FAILED(pGOMay->Ready_GameObject()))
	{
		MSG_BOX("CGOMay Create Failed");
		Safe_Release(pGOMay);
		return nullptr;
	}

	return pGOMay;
}

void CGOMay::Free()
{
	CGameObject::Free();
}
