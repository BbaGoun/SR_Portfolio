#include "Engine_Define.h"
#include "CCameraMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr():m_pMainCamera(nullptr)
, m_eCameraState(CAMERA_END)
{
	ZeroMemory(m_cameras, sizeof(m_cameras));
}

CCameraMgr::~CCameraMgr()
{
	Free();
}

HRESULT CCameraMgr::Ready_Camera(CAMERA_STATE _eID, CCamera* _pCamera)
{
	if (nullptr == _pCamera)
		return E_FAIL;

	m_cameras[_eID] = _pCamera;

	return S_OK;
}

void CCameraMgr::UpdateMainCameraInfo(_matrix* _matView, _matrix* _matProj)
{
	m_tCameraInfo.matView = *_matView;
	m_tCameraInfo.matProj = *_matProj;
}

HRESULT CCameraMgr::SetMainCamera(CAMERA_STATE _eID)
{
	if (m_cameras[_eID] == nullptr)
		return E_FAIL;

	m_pMainCamera = m_cameras[_eID];
	m_eCameraState = _eID;

	return S_OK;
}

void CCameraMgr::Free()
{
}
