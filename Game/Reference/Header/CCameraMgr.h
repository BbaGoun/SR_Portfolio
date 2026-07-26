#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine);

class ENGINE_DLL CCameraMgr :
    public CBase
{
    DECLARE_SINGLETON(CCameraMgr)


private:
	explicit	CCameraMgr();
	~CCameraMgr() override;

public:
	void AddCamera() { m_vecCameraInfos.push_back({}); }
	void UpdateCameraInfo(int _index, _matrix* _matView, _matrix* _matProj);
	const CameraInfo& GetCameraInfo(int _index) { return m_vecCameraInfos[_index]; }

private:
	vector<CameraInfo> m_vecCameraInfos;

private:
	// CBase을(를) 통해 상속됨
	void Free() override;
};

END

