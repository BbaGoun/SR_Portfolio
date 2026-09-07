#include "Engine_Define.h"
#include "CCameraMgr.h"
#include "CGameObject.h"
#include "CManagement.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr():m_pMainCamera(nullptr)
, m_eCameraState(CAMERA_END), m_ePreCameraState(CAMERA_END)
, m_bRePlay(false)
,m_pCart(nullptr)
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
void CCameraMgr::UpdateClosedRePlayCam()
{
	if (m_bRePlay == false || m_pCart == nullptr)
		return;
	m_eCameraState = CAMERA_TRACK;
	float fMinDist = FLT_MAX;
	_vec3 vCartPos;
	m_pCart->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	for (auto* pCams : m_vecRePlayCams)
	{
		_vec3 vCamPos,vDeltaPos;
		pCams->Get_Transform()->Get_Info(INFO_POS, &vCamPos);
		vDeltaPos = vCartPos - vCamPos;
		if (fMinDist > D3DXVec3Length(&vDeltaPos))
		{
			fMinDist = D3DXVec3Length(&vDeltaPos);
			m_pMainCamera = pCams;
		}
	}
	// 플레이어와 가장 가까운 카메라로 설정
	//m_pMainCamera = m_cameras[_eID];
	//m_eCameraState = _eID;
}

void CCameraMgr::SetRePlay(bool bRePlay)
{
	m_bRePlay = bRePlay;
	if (bRePlay == false)
		SetMainCamera(m_ePreCameraState);
	m_ePreCameraState = m_eCameraState;
	UpdateClosedRePlayCam();
}
HRESULT CCameraMgr::SetMainCamera(CAMERA_STATE _eID)
{
	if (m_cameras[_eID] == nullptr)
		return E_FAIL;

	m_pMainCamera = m_cameras[_eID];

	m_ePreCameraState = m_eCameraState;
	m_eCameraState = _eID;

	return S_OK;
}

void CCameraMgr::Free()
{
}
