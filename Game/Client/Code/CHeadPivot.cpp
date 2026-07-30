#include "pch.h"
#include "CHeadPivot.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CCameraMgr.h"
#include "CDInputMgr.h"

CHeadPivot::CHeadPivot(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CHeadPivot::CHeadPivot(const CGameObject& rhs) : CGameObject(rhs)
{
}

CHeadPivot::~CHeadPivot()
{
}

HRESULT CHeadPivot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0.f, 8.f, 1.5f });

	m_fPitch = 0;

	return S_OK;
}

_int CHeadPivot::Update_GameObject(const _float& fTimeDelta)
{
	Mouse_Input(fTimeDelta);

	return CGameObject::Update_GameObject(fTimeDelta);
}

void CHeadPivot::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHeadPivot::Mouse_Input(const _float& fTimeDelta)
{
	_matrix* matWorld = m_pTransformCom->Get_World();

	_long	dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		m_fPitch = clampT(m_fPitch + dwMouseMove / 10.f, -89.f, 89.f);
		_vec3	vRight = { 1, 0, 0 };
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &vRight, D3DXToRadian(m_fPitch));

		m_pTransformCom->Set_Quaternion(&q);
	}
}

CHeadPivot* CHeadPivot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeadPivot* pObj = new CHeadPivot(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CHeadPivot::Free()
{
	CGameObject::Free();
}
