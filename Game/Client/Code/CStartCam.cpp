#include "pch.h"
#include "CStartCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"
#include "CCart.h"
#include "CPlayTimeMgr.h"

CStartCam::CStartCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CStartCam::CStartCam(const CStartCam& rhs)
	: CCamera(rhs)
{
}

CStartCam::~CStartCam()
{
}

HRESULT CStartCam::Ready_GameObject(const _vec3& pEye,
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
void CStartCam::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CTransform* pTrans = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart")->Get_Component<CTransform>();

		if (pTrans == nullptr)
			return;

		_vec3	vMyPos;
		_vec3	vMyRight;
		_vec3	vPlayerPos;
		_vec3	vPlayerRight;
		_vec3	vPlayerUp = { 0,1,0 };
		_vec3	vPlayerLOOK;

		pTrans->Get_Info(INFO_POS, &vPlayerPos);
		pTrans->Get_Info(INFO_RIGHT, &vPlayerRight);
		pTrans->Get_Info(INFO_LOOK, &vPlayerLOOK);

		m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
		vMyRight = vPlayerRight * -1;

		
		// 카메라 아래 움직임
		if (vMyPos.y >= vPlayerPos.y + 5.f)
			m_vForce.y = -3.f;
		else
			m_vForce.y = 0.f;

		// 카메라 좌우 움직임
		m_fLRMoveTime += fFixedDeltaTime;
		if (m_fLRMoveTime < 2.0f)
			m_vForce.x += -vMyRight.x * fFixedDeltaTime * 3.f;
		else if (m_fLRMoveTime <= 4.f)
			m_vForce.x += vMyRight.x * fFixedDeltaTime * 8.f;
		else if (m_fLRMoveTime <= 6.f)
		{
			m_vForce.x = vMyRight.x * fFixedDeltaTime * 25.f;

			// 캐릭터 클로즈업 구간
			_vec3 vDeltaPos = vPlayerPos - vMyPos;
			float fDeltaPos = D3DXVec3Length(&vDeltaPos);

			if (fDeltaPos > 10.f)
				m_vForce.z = vDeltaPos.z * 0.5f;
			else
				m_vForce.z = 0.f;
		}
		else
		{
			// 카메라 변경 후 시작
			CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FOLLOW_SMOOTH);
			CPlayTimeMgr::GetInstance()->SetRaceStart();
		}

		m_pTransformCom->Move_Pos(&m_vForce, 1.f, fFixedDeltaTime);
		m_pTransformCom->Get_Info(INFO_POS, &m_vEye);
		m_vAt = vPlayerPos;
		m_vAt.y += 2.f;
		m_vUp = vPlayerUp;
	}
}


_int CStartCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {

	}
	return 0;
}

void CStartCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CStartCam* CStartCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CStartCam* pCamera = new CStartCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CStartCam::Free()
{
	CCamera::Free();
}
