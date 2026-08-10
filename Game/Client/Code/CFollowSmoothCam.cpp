#include "pch.h"
#include "CFollowSmoothCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"
#include "CCart.h"

CFollowSmoothCam::CFollowSmoothCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFollowSmoothCam::CFollowSmoothCam(const CFollowSmoothCam& rhs)
	: CCamera(rhs)
{
}

CFollowSmoothCam::~CFollowSmoothCam()
{
}

HRESULT CFollowSmoothCam::Ready_GameObject(const _vec3& pEye,
	const _vec3& pAt,
	const _vec3& pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye	= pEye;
	m_vAt	= pAt;
	m_vUp	= pUp;

	m_fFov		= fFov;
	m_fAspect	= fAspect;
	m_fNear		= fNear;
	m_fFar		= fFar;

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;
	m_pTransformCom->Set_Pos(m_vEye);
	m_fYaw = 0;
	m_fDistScale = 1;
	m_fBackDistance = 0.f;
	return S_OK;
}
void CFollowSmoothCam::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Cart", L"Com_Transform");
		if (pCom == nullptr)
			return;

		CTransform* pTrans = static_cast<CTransform*>(pCom);
		_vec3	vMyPos;
		_vec3	vPlayerPos;
		_vec3	vPlayerUp = { 0,1,0 };
		_vec3	vPlayerLOOK;

		pTrans->Get_Info(INFO_POS, &vPlayerPos);
		//pTrans->Get_Info(INFO_UP, &vPlayerUp);
		pTrans->Get_Info(INFO_LOOK, &vPlayerLOOK);

		m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

		_vec3	vTargetPos = vPlayerPos + (vPlayerUp * 10) + (vPlayerLOOK * -12);

		_vec3	vPlayerForce = pTrans->Get_Owner()->Get_Force();
		_vec3	vDeltaPos = vTargetPos - vMyPos;

		float	fDeltaPos = D3DXVec3Length(&vDeltaPos);
		float	fChaseSpeed = fDeltaPos * 4;

		if (D3DXVec3Dot(&vPlayerLOOK, &vPlayerForce) >= 0.2)
			m_pTransformCom->Chase_Target(&vTargetPos, fChaseSpeed, fFixedDeltaTime);
		else if (D3DXVec3Dot(&vPlayerLOOK, &vPlayerForce) < 0)
			m_pTransformCom->Chase_Target(&vTargetPos, fChaseSpeed * 2, fFixedDeltaTime);

		m_pTransformCom->Get_Info(INFO_POS, &m_vEye);
		m_vEye.y = vTargetPos.y;
		m_vAt = vPlayerPos;
		m_vUp = vPlayerUp;
	}
}


_int CFollowSmoothCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {

	}
	return 0;
}

void CFollowSmoothCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CFollowSmoothCam* CFollowSmoothCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CFollowSmoothCam* pCamera = new CFollowSmoothCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CFollowSmoothCam::Free()
{
	CCamera::Free();
}
