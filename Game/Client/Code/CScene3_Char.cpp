#include "pch.h"
#include "CScene3_Char.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
CScene3_Char::CScene3_Char(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
	:CGameObject(pGraphicDev), m_eSlotNum(eID)
{
}

CScene3_Char::CScene3_Char(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CScene3_Char::~CScene3_Char()
{
}

HRESULT CScene3_Char::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0, 0, 2 });
	m_pTransformCom->Set_Scale({ 3, 3, 3 });
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CQuadrangularPrism*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_QuadrangularPrism"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	m_bTurn = false;

	return S_OK;
}

void CScene3_Char::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	m_pTransformCom->Set_Quaternion(&q);
}

_int CScene3_Char::Update_GameObject(const _float& fDeltaTime)
{
	TCHAR   szFileName[128] = L"";
	wsprintf(szFileName, L"CharSlot%d", m_eSlotNum);
	CRenderer::GetInstance()->Add_RenderTargetGroup(szFileName, this);

	if (m_bTurn)
		m_vRotation.y += fDeltaTime;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_Char::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_Char::Render_GameObject()
{
	_matrix OldView, OldProj, matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProj);

	_vec3 vEye, vAt, vUp;
	vEye = { 0, 0, -15 };
	vAt = { 0, 0, 0 };
	vUp = { 0, 1, 0 };
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	_float fAspect = (_float)vp.Width / (_float)vp.Height;

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60), fAspect, 1.f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetTexture(0, nullptr);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProj);
}

CScene3_Char* CScene3_Char::Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID)
{
	CScene3_Char* pObj = new CScene3_Char(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_Char Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene3_Char::Free()
{
	CGameObject::Free();
}
