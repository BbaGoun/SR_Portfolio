#include "pch.h"
#include "CFixedThirdPerCam.h"
#include "CCameraMgr.h"

CFixedThirdPerCam::CFixedThirdPerCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFixedThirdPerCam::CFixedThirdPerCam(const CFixedThirdPerCam& rhs)
	: CCamera(rhs)
{
}

CFixedThirdPerCam::~CFixedThirdPerCam()
{
}

HRESULT CFixedThirdPerCam::Ready_GameObject(const _vec3& pEye,
										const _vec3& pAt,
										const _vec3& pUp,
										const _float& fFov, 
										const _float& fAspect, 
										const _float& fNear, 
										const _float& fFar)
{
	m_vEye = pEye;
	m_vAt  = pAt;
	m_vUp  = pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;

	return S_OK;
}

_int CFixedThirdPerCam::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFixedThirdPerCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_POS, &m_vEye);
		m_pTransformCom->Get_Info(INFO_UP, &m_vUp);
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

		m_vAt = m_vEye;
		m_vEye -= vLook * 5.f;
		CCamera::LateUpdate_GameObject(fTimeDelta);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

CFixedThirdPerCam* CFixedThirdPerCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CFixedThirdPerCam* pCamera = new CFixedThirdPerCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CFixedThirdPerCam::Free()
{
	CCamera::Free();
}
