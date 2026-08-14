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
#include "CCollisionBox.h"
#include "CBoostWind.h"
#include "CUI_Exit.h"
#include "CBoostJet.h"
#include "CHUD_Main.h"
#include "CHUD_Gage.h"
#include "CHUD_Num.h"
#include "CLand3.h"
#include "CRainBow_Cloud.h"
#include "CUI_Rainbow.h"

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

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;

	return S_OK;
}

void CCollisionTest::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	// 충돌 처리
	CCollider* pCartCollider = static_cast<CCollider*>
		(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Cart", L"Com_Collider"));
	//CCollider* pCartSphereCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Cart", L"Com_SphereCollider"));
	
	CCollider* pBoxCollider = static_cast<CCollider*>
		(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_CollisionBox", L"Com_Collider"));


	CCollider* pRainBowCollider = static_cast<CCollider*>
		(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Rainbow_Cloud", L"Com_Collider"));
	

	CCollisionMgr::GetInstance()->Collision(pBoxCollider, pCartCollider);
	CCollisionMgr::GetInstance()->Collision(pRainBowCollider, pCartCollider);



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
	// 게임 오브젝트
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	// # 카트
	CGameObject* pCart = CCart::Create(m_pGraphicDev);

	if (pCart == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Cart", pCart)))
		return E_FAIL;
	pCart->SetLayer(pGameObjectLayer);


	// ## 카트 몸체
	CGameObject* pCartBody = CCartBody::Create(m_pGraphicDev);

	if (pCartBody == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CartBody", pCartBody)))
		return E_FAIL;

	pCart->Set_Child(pCartBody);

	CGameObject* pGameObject = nullptr;
	// ## 카트 몸체1
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pCartBody->Set_Child(pGameObject);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,1.5,1 });
	pGameObject->Get_Transform()->Set_Pos({ 0,0,-3 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody1", pGameObject)))
		return E_FAIL;
	
	
	// ## 카트 몸체2
	pGameObject = CCartBody2::Create(m_pGraphicDev);
	pCartBody->Set_Child(pGameObject);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,0.5,0.5 });
	pGameObject->Get_Transform()->Set_Pos({ 0,1,-1.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody2", pGameObject)))
		return E_FAIL;
	
	
	// ## 카트 몸체3
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pCartBody->Set_Child(pGameObject);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,1,0.5 });
	pGameObject->Get_Transform()->Set_Pos({ 0,-0.5,-1.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody3", pGameObject)))
		return E_FAIL;
	
	
	// ## 카트 몸체4
	pGameObject = CCartBody1::Create(m_pGraphicDev);
	pCartBody->Set_Child(pGameObject);
	pGameObject->Get_Transform()->Set_Scale({ 2.5,1.f,4 });
	pGameObject->Get_Transform()->Set_Pos({ 0,-0.75,0.5 });
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CCartBody4", pGameObject)))
		return E_FAIL;
	


	// ## 왼쪽 앞바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_FL);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_FL", pGameObject)))
		return E_FAIL;
	
	pCart->Set_Child(pGameObject);
	// ## 오른쪽 앞바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_FR);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_FR", pGameObject)))
		return E_FAIL;
	
	pCart->Set_Child(pGameObject);
	
	// ## 왼쪽 뒷바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BL);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BL", pGameObject)))
		return E_FAIL;
	
	pCart->Set_Child(pGameObject);
	
	// ## 오른쪽 뒷바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BR);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BR", pGameObject)))
		return E_FAIL;
	
	pCart->Set_Child(pGameObject);

	// ## 부스터 왼쪽1 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_L1);

	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"BoostWindL1", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);

	// ## 부스터 왼쪽2 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_L2);
	
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"BoostWindL2", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);
	
	// ## 부스터 오른쪽1 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_R1);

	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"BoostWindR1", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);
	// ## 부스터 오른쪽2 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_R2);
	
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"BoostWindR2", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);

	// ## 부스터 제트 이펙트
	pGameObject = CBoostJet::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"BoostJet", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);

	//// # 플레이어 따라다니는 3인칭 카메라
	_vec3 vEye, vAt, vUp, vLook;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vAt);
	pCart->Get_Transform()->Get_Info(INFO_UP, &vUp);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	vEye = vAt + (vUp * 10) + (vLook * -20);
	pGameObject = CFollowSmoothCam::Create(m_pGraphicDev, vEye, vAt, vUp);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_FollowSmoothCam", pGameObject)))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_FOLLOW_SMOOTH,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FOLLOW_SMOOTH)))
		return E_FAIL;


	// # 트랙
	for (int i = 0; i < 40; ++i)
	{
		CGameObject* pBox = CCartBody1::Create(m_pGraphicDev);

		if (pBox == nullptr)
			return E_FAIL;
		pBox->Get_Transform()->Set_Scale({ 2,2,2 });
		if (i < 20)
		{
			pBox->Get_Transform()->Set_Pos({ 100 * cosf(2 * D3DX_PI / 40 * i), 0.f, 100 + 100 * sinf(2 * D3DX_PI / 40 * i) });
		}
		else
		{
			pBox->Get_Transform()->Set_Pos({ 100 * cosf(2 * D3DX_PI / 40 * i), 0.f, -100 + 100 * sinf(2 * D3DX_PI / 40 * i) });
		}
		TCHAR szBuff[32];
		wsprintf(szBuff, L"Obj_Box%d", i);
		if (FAILED(pGameObjectLayer->Add_GameObject(szBuff, pBox)))
			return E_FAIL;
	}
	for (int i = 0; i < 40; ++i)
	{
		CGameObject* pBox = CCartBody1::Create(m_pGraphicDev);

		if (pBox == nullptr)
			return E_FAIL;
		pBox->Get_Transform()->Set_Scale({ 2,2,2 });
		if (i < 20)
		{
			pBox->Get_Transform()->Set_Pos({ -100.f, 0.f,100 - 10.f * i });
		}
		else
		{
			pBox->Get_Transform()->Set_Pos({ 100.f, 0.f,100 - 10.f * (i - 20) });
		}

		TCHAR szBuff[32];
		wsprintf(szBuff, L"Obj_Box%d", i);
		if (FAILED(pGameObjectLayer->Add_GameObject(szBuff, pBox)))
			return E_FAIL;
	}

	CGameObject* pBox = CCollisionBox::Create(m_pGraphicDev);

	if (pBox == nullptr)
		return E_FAIL;
	pBox->Get_Transform()->Set_Pos({ -200.f, 1.f,200.f  });
	
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CollisionBox", pBox)))
		return E_FAIL;


	//// 무지개 구름 이펙트
	//pGameObject = CRainbow_Cloud::Create(m_pGraphicDev);
	//
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//if (FAILED(pGameObjectLayer->Add_GameObject(L"Rainbow_Cloud", pGameObject)))
	//	return E_FAIL;
	//pGameObject->SetLayer(pGameObjectLayer);

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

	//pEnvObject = CLand2::Create(m_pGraphicDev);
	//
	//if (pEnvObject == nullptr)
	//	return E_FAIL;
	//
	//if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land2", pEnvObject)))
	//	return E_FAIL;
	pEnvObject = CLand3::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;

	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land3", pEnvObject)))
		return E_FAIL;
	return S_OK;
}


HRESULT CCollisionTest::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();

	if (pUILayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"UI", pUILayer });

	CGameObject* pUIObject = nullptr;

	// UI_HUDMain
	pUIObject = CHUD_Main::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_HUDMain", pUIObject)))
		return E_FAIL;

	// UI_HUDGage
	pUIObject = CHUD_Gage::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_HUDGage", pUIObject)))
		return E_FAIL;

	// UI_HUDNum
	pUIObject = CHUD_Num::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_HUDNum", pUIObject)))
		return E_FAIL;

	// UI_Rainbow
	pUIObject = CUI_Rainbow::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Rainbow", pUIObject)))
		return E_FAIL;


	return S_OK;
}

void CCollisionTest::Free()
{
	CScene::Free();
}
