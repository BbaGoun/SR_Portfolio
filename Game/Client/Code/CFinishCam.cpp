#include "pch.h"
#include "CFinishCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"
#include "CCart.h"
#include "CPlayTimeMgr.h"

CFinishCam::CFinishCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFinishCam::CFinishCam(const CFinishCam& rhs)
	: CCamera(rhs)
{
}

CFinishCam::~CFinishCam()
{
}

HRESULT CFinishCam::Ready_GameObject(const _vec3& pEye,
	const _vec3& pAt,
	const _vec3& pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = pEye;
	m_vAt = pAt;
	m_vUp = pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_vEye);
	m_fYaw = 0;
	m_fDistScale = 1;
	m_fBackDistance = 0.f;
	m_fDeltaAngle = -45.f;
	m_vRotation.y = -45.f;
	return S_OK;
}
void CFinishCam::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CTransform* pTrans = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart")->Get_Component<CTransform>();

		if (pTrans == nullptr)
			return;

		_vec3	vPlayerPos, vPlayerRight, vPlayerUp;
		pTrans->Get_Info(INFO_POS, &vPlayerPos);
		pTrans->Get_Info(INFO_RIGHT, &vPlayerRight);
		pTrans->Get_Info(INFO_UP, &vPlayerUp);

		if (m_vRotation.y > -45.f)
			m_fDeltaAngle = -45.f;
		if (m_vRotation.y < -135.f)
			m_fDeltaAngle = 45.f;

		m_vRotation.y += m_fDeltaAngle * fFixedDeltaTime;
		_matrix matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_vRotation.y));

		m_vEye = vPlayerRight * 20.f + vPlayerUp * 15.f;// _vec3({ 30, 15, 0 });
		D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRot);

		m_vEye += vPlayerPos;
		m_vAt = vPlayerPos;
	}
}


_int CFinishCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {

	}
	return 0;
}

void CFinishCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CFinishCam* CFinishCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CFinishCam* pCamera = new CFinishCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CFinishCam::Free()
{
	CCamera::Free();
}
