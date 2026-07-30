#include "pch.h"
#include "CFrontThirdPerCam.h"
#include "CCameraMgr.h"

CFrontThirdPerCam::CFrontThirdPerCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFrontThirdPerCam::CFrontThirdPerCam(const CFrontThirdPerCam& rhs)
	: CCamera(rhs)
{
}

CFrontThirdPerCam::~CFrontThirdPerCam()
{
}

HRESULT CFrontThirdPerCam::Ready_GameObject(
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

_int CFrontThirdPerCam::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFrontThirdPerCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		_vec3 vLook;
		m_pParent->Get_Transform()->Get_Info(INFO_POS, &m_vEye);
		m_pParent->Get_Transform()->Get_Info(INFO_UP, &m_vUp);
		m_pParent->Get_Transform()->Get_Info(INFO_LOOK, &vLook);

		m_vAt = m_vEye;
		m_vEye = m_vEye + vLook * 20;

		CCamera::LateUpdate_GameObject(fTimeDelta);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CFrontThirdPerCam* CFrontThirdPerCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CFrontThirdPerCam* pCamera = new CFrontThirdPerCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CFrontThirdPerCam::Free()
{
	CCamera::Free();
}
