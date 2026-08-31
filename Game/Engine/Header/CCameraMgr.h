#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CCamera.h"

BEGIN(Engine);

enum CAMERA_STATE{
	CAMERA_FIRST,
	CAMERA_BACK_THIRD,
	CAMERA_FRONT_THIRD,
	CAMERA_TOP_VIEW,
	CAMERA_FOLLOW_SMOOTH,
	CAMERA_DYNAMIC,
	CAMERA_START,
	CAMERA_END
};

class ENGINE_DLL CCameraMgr :
    public CBase
{
    DECLARE_SINGLETON(CCameraMgr)

private:
	explicit	CCameraMgr();
	~CCameraMgr() override;

public:
	HRESULT Ready_Camera(CAMERA_STATE _eID, CCamera* _pCamera);
	void UpdateMainCameraInfo(_matrix* _matView, _matrix* _matProj);
	HRESULT SetMainCamera(CAMERA_STATE _eID);
	CCamera* GetMainCamera() { return m_pMainCamera; }
	CAMERA_STATE GetCamerState() { return m_eCameraState; }
	const CameraInfo& GetCameraInfo() { return m_tCameraInfo; }
	bool CheckIsMainCamera(CCamera* _pCamera) { return _pCamera == m_pMainCamera; }


private:
	CCamera* m_cameras[CAMERA_END];
	CCamera* m_pMainCamera;
	CAMERA_STATE m_eCameraState;
	CameraInfo m_tCameraInfo;

private:
	// CBase을(를) 통해 상속됨
	void Free() override;
};

END

