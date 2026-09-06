#include "pch.h"
#include "CLeftSideMirrorCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"
#include "CCart.h"

CLeftSideMirrorCam::CLeftSideMirrorCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CLeftSideMirrorCam::CLeftSideMirrorCam(const CLeftSideMirrorCam& rhs)
	: CCamera(rhs)
{
}

CLeftSideMirrorCam::~CLeftSideMirrorCam()
{
}

HRESULT CLeftSideMirrorCam::Ready_GameObject(const _vec3& pEye,
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

	// AtYOffset : 4
	// Up: 8.5
	// Back : 15
	// Fov : 45
	// 플레이어의 적정 속도가 정해진 후 가능할 듯

	m_fAtYOffset = 4.f;
	m_fUpDistance = 8.5f;
	m_fBackDistance = 15.f;
	return S_OK;
}
void CLeftSideMirrorCam::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CTransform* pTrans = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart")->Get_Component<CTransform>();

	if (pTrans == nullptr)
		return;

	_vec3	vMyPos, vPlayerPos, vPlayerUp, vPlayerLOOK, vPlayerRight;

	pTrans->Get_Info(INFO_POS, &vPlayerPos);
	pTrans->Get_Info(INFO_UP, &vPlayerUp);
	pTrans->Get_Info(INFO_LOOK, &vPlayerLOOK);
	pTrans->Get_Info(INFO_RIGHT, &vPlayerRight);

	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

	vMyPos = vPlayerPos + -vPlayerRight * 2.f;
	m_pTransformCom->Set_Pos(vMyPos);

	m_pTransformCom->Get_Info(INFO_POS, &m_vEye);
	m_vAt = vMyPos + vPlayerLOOK * - 2.f;
	m_vUp = vPlayerUp;
	
}


_int CLeftSideMirrorCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {

	}
	return 0;
}

void CLeftSideMirrorCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CLeftSideMirrorCam* CLeftSideMirrorCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CLeftSideMirrorCam* pCamera = new CLeftSideMirrorCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CLeftSideMirrorCam::Free()
{
	CCamera::Free();
}
