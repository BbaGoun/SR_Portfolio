#include "pch.h"
#include "CTrackCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"
#include "CCart.h"
#include "CPlayTimeMgr.h"

CTrackCam::CTrackCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CTrackCam::CTrackCam(const CTrackCam& rhs)
	: CCamera(rhs)
{
}

CTrackCam::~CTrackCam()
{
}

HRESULT CTrackCam::Ready_GameObject(const _vec3& pEye,
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
	m_fLRMoveTime = 0.f;

	return S_OK;
}
void CTrackCam::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CTransform* pTrans = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart")->Get_Component<CTransform>();

		if (pTrans == nullptr)
			return;
		_vec3	vPlayerPos;
		pTrans->Get_Info(INFO_POS, &vPlayerPos);
		m_vAt = vPlayerPos;
	}
}


_int CTrackCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {

	}
	return 0;
}

void CTrackCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CTrackCam* CTrackCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CTrackCam* pCamera = new CTrackCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CTrackCam::Free()
{
	CCamera::Free();
}
