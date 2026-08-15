#include "pch.h"
#include "CScene_Test.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CSkyBox.h"
#include "CCamera.h"
#include "CGOCody.h"
#include "CHeadPivot.h"
#include "CFirstPerCam.h"
#include "CBackThirdPerCam.h"
#include "CFrontThirdPerCam.h"
#include "CCameraMgr.h"
#include "CCodyBody.h"
#include "CLand2.h"
#include "CTopViewCam.h"

CScene_Test::CScene_Test(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CScene_Test::~CScene_Test()
{
}

HRESULT CScene_Test::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;

	return S_OK;
}

void CScene_Test::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	// 충돌 처리
}

_int CScene_Test::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CScene_Test::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CScene_Test::Render_Scene()
{
	//CScene::Render_Scene();
}

CScene_Test* CScene_Test::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Test* pScene = new CScene_Test(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CScene_Test Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CScene_Test::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_Test::Ready_GameLogic_Layer()
{
	// 게임 오브젝트
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	// # 플레이어
	CGameObject* pPlayer = CGOCody::Create(m_pGraphicDev);

	if (pPlayer == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Player", pPlayer)))
		return E_FAIL;

	// ## 플레이어 머리
	CGameObject* pHead = CHeadPivot::Create(m_pGraphicDev);

	if (pHead == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_HeadPivot", pHead)))
		return E_FAIL;

	pPlayer->Set_Child(pHead);

	CGameObject* pGameObject = nullptr;

	// ## 플레이어 몸체
	pGameObject = CCodyBody::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Body", pGameObject)))
		return E_FAIL;

	pPlayer->Set_Child(pGameObject);

	// ### 머리에 달린 카메라
	pGameObject = CFirstPerCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_FirstCam", pGameObject)))
		return E_FAIL;

	pHead->Set_Child(pGameObject);

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_FIRST,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FIRST)))
		return E_FAIL;

	// ### 플레이어 뒤에 달린 3인칭 카메라
	pGameObject = CBackThirdPerCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_BackThirdCam", pGameObject)))
		return E_FAIL;

	pHead->Set_Child(pGameObject);
	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_BACK_THIRD,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	// ### 플레이어 앞에 달린 3인칭 카메라
	pGameObject = CFrontThirdPerCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_FrontThirdCam", pGameObject)))
		return E_FAIL;

	pHead->Set_Child(pGameObject);
	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_FRONT_THIRD,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	// # 탑뷰 3인칭 카메라
	pGameObject = CTopViewCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_TopViewCam", pGameObject)))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_TOP_VIEW,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Test::Ready_Environment_Layer()
{
	CLayer* pEnvironmentLayer = CLayer::Create();

	if (pEnvironmentLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"Environment", pEnvironmentLayer });

	CGameObject* pEnvObject = nullptr;
	pEnvObject = CSkyBox::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;

	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_SkyBox", pEnvObject)))
		return E_FAIL;

	//pEnvObject = CLand::Create(m_pGraphicDev);

	//if (pEnvObject == nullptr)
	//	return E_FAIL;

	//if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land", pEnvObject)))
	//	return E_FAIL;

	pEnvObject = CLand2::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;

	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land2", pEnvObject)))
		return E_FAIL;

	return S_OK;
}

void CScene_Test::Free()
{
	CScene::Free();
}
