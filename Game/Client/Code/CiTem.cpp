#include "pch.h"
#include "CItem.h"
#include "CMissile.h"
#include "CTopViewCam.h"
#include "CCameraMgr.h"
#include "CBox.h"
#include "CFollowSmoothCam.h"
#include "CDynamicCamera.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Scene()
{
	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	return S_OK;
}

void CItem::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);
}

_int CItem::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CItem::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CItem::Render_Scene()
{
	//CScene::Render_Scene();
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItem* pScene = new CItem(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CItem Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CItem::Ready_GameLogic_Layer()
{
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	
	CGameObject* pMissile = CMissile::Create(m_pGraphicDev);

	if (pMissile == nullptr)
		return E_FAIL;

		if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Player", pMissile)))	// 카메라가 플레이어를 쫒아가게 되어 있으니 L"Obj_Player" 로 설정
		return E_FAIL;

	CGameObject* pBox = CBox::Create(m_pGraphicDev);

	if (pBox == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Box", pBox)))	
		return E_FAIL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 다이나믹 카메라
	_vec3 vEye = { 0.f, 30.f, -30.f };
	_vec3 vAt = { 0.f, 0.f, 100.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };

	CGameObject* pDynamicCam = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
	
	if (pDynamicCam == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_DynamicCamera", pDynamicCam)))
		return E_FAIL;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// (테스트용으로 아이템에 카메라 만듬)
	//CGameObject* pTopViewCam = CTopViewCam::Create(m_pGraphicDev);

	//if (pTopViewCam == nullptr)
	//	return E_FAIL;

	//if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_TopViewCam", pTopViewCam)))
	//	return E_FAIL;

	//if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_TOP_VIEW,
	//	static_cast<CCamera*>(pTopViewCam))))
	//	return E_FAIL;

	//if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_TOP_VIEW)))
	//	return E_FAIL;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

void CItem::Free()
{
	CScene::Free();
}
