#include "CCamera.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"

CCamera::CCamera() : CComponent()
{
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
{

}

CCamera::CCamera(const CCamera& rhs) :CComponent(rhs)
, m_vAngle(rhs.m_vAngle), m_fFov(rhs.m_fFov)
, m_fAspect(rhs.m_fAspect), m_fNear(rhs.m_fNear)
, m_fFar(rhs.m_fFar)
{

}

CCamera::~CCamera()
{
}

_int CCamera::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CCamera::LateUpdate_Component(const _float& fTimeDelta)
{
}

void CCamera::SetCamera_BeforeRender()
{
	CTransform* pTransComp = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	if (pTransComp == nullptr)
		pTransComp = static_cast<CTransform*>(m_pOwner->Get_Component(ID_STATIC, L"Com_Transform"));

	_vec3 camPos = pTransComp->m_vInfo[INFO_POS];
	_vec3 vAt = camPos;

	_vec3 vInfoRight, vInfoUp, vInfoLook;
	pTransComp->Get_Info(INFO_RIGHT, &vInfoRight);
	pTransComp->Get_Info(INFO_UP, &vInfoUp);
	pTransComp->Get_Info(INFO_LOOK, &vInfoLook);
	D3DXVec3Normalize(&vInfoRight, &vInfoRight);
	D3DXVec3Normalize(&vInfoUp, &vInfoUp);
	D3DXVec3Normalize(&vInfoLook, &vInfoLook);

	// 카메라가 대상으로부터 떨어지는 정도
	_vec3 vOffset = -vInfoLook * 10;

	// 회전	
	D3DXQUATERNION qRotation, qYaw, qPitch;
	D3DXQuaternionRotationAxis(&qYaw, &vInfoUp, D3DXToRadian(m_vAngle.y));
	D3DXQuaternionRotationAxis(&qPitch, &vInfoRight, D3DXToRadian(m_vAngle.x));

	qRotation = qPitch * qYaw;   // 항상 같은 순서로 재조립

	_matrix matRotQ;
	D3DXMatrixRotationQuaternion(&matRotQ, &qRotation);
	D3DXVec3TransformCoord(&vOffset, &vOffset, &matRotQ);

	camPos += vOffset;

	_vec3 vUp = vInfoUp;

	_matrix matView, matProj;

	D3DXMatrixLookAtLH(&matView, &camPos, &vAt, &vUp);

	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DXToRadian(m_fFov),
		m_fAspect,
		m_fNear,
		m_fFar
	);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

HRESULT CCamera::Ready_Camera()
{
	m_vAngle = { 0, 0, 0};

	m_fFov = 60;
	m_fAspect = (float)16 / (float)9;
	m_fNear = 1;
	m_fFar = 2000;

	return S_OK;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCamera* pCameraComp = new CCamera(pGraphicDev);

	if (FAILED(pCameraComp->Ready_Camera())) {
		Safe_Release(pCameraComp);
		MSG_BOX("CCamera Create Fail");
		return nullptr;
	}

	return pCameraComp;
}

CComponent* CCamera::Clone()
{
	return new CCamera(*this);
}

void CCamera::Free()
{
	CComponent::Free();
}