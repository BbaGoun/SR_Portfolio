#include "Engine_Define.h"
#include "CCameraMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{

}

CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::UpdateCameraInfo(int _index, _matrix* _matView, _matrix* _matProj)
{
	m_vecCameraInfos[_index].matView = *_matView;
	m_vecCameraInfos[_index].matProj = *_matProj;
}

void CCameraMgr::Free()
{
}
