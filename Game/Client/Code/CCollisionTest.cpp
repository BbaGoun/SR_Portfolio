#include "pch.h"
#include "CCollisionTest.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CSkyBox.h"
#include "CBackThirdPerCam.h"
#include "CCameraMgr.h"
#include "CLand2.h"
#include "CTopViewCam.h"
#include "CCart.h"
#include "CWheel.h"
#include "CCartBody.h"

CCollisionTest::CCollisionTest(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CCollisionTest::~CCollisionTest()
{
}

HRESULT CCollisionTest::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;

	return S_OK;
}

void CCollisionTest::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	// Ãæµ¹ Ã³¸®
}

_int CCollisionTest::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CCollisionTest::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CCollisionTest::Render_Scene()
{
	//CScene::Render_Scene();
}

CCollisionTest* CCollisionTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollisionTest* pScene = new CCollisionTest(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CCollisionTest Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CCollisionTest::Ready_Prototype()
{
	return S_OK;
}

HRESULT CCollisionTest::Ready_GameLogic_Layer()
{
	// °ÔÀÓ ¿ÀºêÁ§Æ®
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	// # Ä«Æ®
	CGameObject* pCart = CCart::Create(m_pGraphicDev);

	if (pCart == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Cart", pCart)))
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	// ## Ä«Æ® ¸öÃ¼
	pGameObject = CCartBody::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CartBody", pGameObject)))
		return E_FAIL;

	pCart->Set_Child(pGameObject);

	// ## ¿ÞÂÊ ¾Õ¹ÙÄû
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_FL);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_FL", pGameObject)))
		return E_FAIL;

	pCart->Set_Child(pGameObject);
	// ## ¿À¸¥ÂÊ ¾Õ¹ÙÄû
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_FR);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_FR", pGameObject)))
		return E_FAIL;

	pCart->Set_Child(pGameObject);

	// ## ¿ÞÂÊ µÞ¹ÙÄû
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BL);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BL", pGameObject)))
		return E_FAIL;

	pCart->Set_Child(pGameObject);

	// ## ¿À¸¥ÂÊ µÞ¹ÙÄû
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BR);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BR", pGameObject)))
		return E_FAIL;

	pCart->Set_Child(pGameObject);

	// ## Å¾ºä 3ÀÎÄª Ä«¸Þ¶ó
	pGameObject = CTopViewCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_TopViewCam", pGameObject)))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_TOP_VIEW,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;
	pCart->Set_Child(pGameObject);
	return S_OK;
}

HRESULT CCollisionTest::Ready_Environment_Layer()
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
	//
	//if (pEnvObject == nullptr)
	//	return E_FAIL;
	//
	//if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land", pEnvObject)))
	//	return E_FAIL;

	pEnvObject = CLand2::Create(m_pGraphicDev);
	
	if (pEnvObject == nullptr)
		return E_FAIL;
	
	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land2", pEnvObject)))
		return E_FAIL;
	
	return S_OK;
}

void CCollisionTest::Free()
{
	CScene::Free();
}
