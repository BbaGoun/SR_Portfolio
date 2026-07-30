#include "pch.h"
#include "CBackThirdPerCam.h"
#include "CCameraMgr.h"

CBackThirdPerCam::CBackThirdPerCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CBackThirdPerCam::CBackThirdPerCam(const CBackThirdPerCam& rhs)
	: CCamera(rhs)
{
}

CBackThirdPerCam::~CBackThirdPerCam()
{
}

HRESULT CBackThirdPerCam::Ready_GameObject(
	const _vec3& pEye,
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

	return S_OK;
}

_int CBackThirdPerCam::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CBackThirdPerCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		_vec3 vLook;
		m_pParent->Get_Transform()->Get_Info(INFO_POS, &m_vEye);
		m_pParent->Get_Transform()->Get_Info(INFO_UP, &m_vUp);
		m_pParent->Get_Transform()->Get_Info(INFO_LOOK, &vLook);

		m_vAt = m_vEye;
		m_vEye = m_vEye - vLook * 20;

		CCamera::LateUpdate_GameObject(fTimeDelta);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CBackThirdPerCam* CBackThirdPerCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CBackThirdPerCam* pCamera = new CBackThirdPerCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CBackThirdPerCam::Free()
{
	CCamera::Free();
}
