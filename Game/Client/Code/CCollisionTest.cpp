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
#include "CBox.h"
#include "CCollisionMgr.h"
#include "CManagement.h"
#include "CCartBody1.h"
#include "CCartBody2.h"
#include "CFollowSmoothCam.h"

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
	//CCollider* pCartCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_CartBody", L"Com_Collider"));
	//CCollider* pCartSphereCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_CartBody", L"Com_SphereCollider"));
	//CCollider* pBoxCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Box", L"Com_Collider"));
	//CCollisionMgr::GetInstance()->Collision(pCartSphereCollider, pBoxCollider);

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


	// ## Ä«Æ® ¸öÃ¼
	CGameObject* pCartBody = CCartBody::Create(m_pGraphicDev);

	if (pCartBody == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CartBody", pCartBody)))
		return E_FAIL;

	pCart->Set_Child(pCartBody);

	CGameObject* pGameObject = nullptr;
	// ## Ä«Æ® ¸öÃ¼1
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,1.5,1 });
	pGameObject->Get_Transform()->Set_Pos({ 0,0,-3 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody1", pGameObject)))
		return E_FAIL;
	
	pCartBody->Set_Child(pGameObject);

	// ## Ä«Æ® ¸öÃ¼2
	pGameObject = CCartBody2::Create(m_pGraphicDev);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,0.5,0.5 });
	pGameObject->Get_Transform()->Set_Pos({ 0,1,-1.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody2", pGameObject)))
		return E_FAIL;

	pCartBody->Set_Child(pGameObject);

	// ## Ä«Æ® ¸öÃ¼3
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,1,0.5 });
	pGameObject->Get_Transform()->Set_Pos({ 0,-0.5,-1.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody3", pGameObject)))
		return E_FAIL;
	
	pCartBody->Set_Child(pGameObject);

	// ## Ä«Æ® ¸öÃ¼4
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,0.75,6 });
	pGameObject->Get_Transform()->Set_Pos({ 0,-0.5,-1.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody4", pGameObject)))
		return E_FAIL;

	pCartBody->Set_Child(pGameObject);

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

	//// # ÇÃ·¹ÀÌ¾î µû¶ó´Ù´Ï´Â 3ÀÎÄª Ä«¸Þ¶ó
	pGameObject = CFollowSmoothCam::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_FollowSmoothCam", pGameObject)))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_FOLLOW_SMOOTH,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;


	if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FOLLOW_SMOOTH)))
		return E_FAIL;


	// # ¹Ú½º
	CGameObject* pBox = CBox::Create(m_pGraphicDev);

	if (pBox == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Box", pBox)))
		return E_FAIL;

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
