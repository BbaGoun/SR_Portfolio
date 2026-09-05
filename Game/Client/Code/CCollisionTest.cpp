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
#include "CBoostJet.h"
#include "CHUD_Main.h"
#include "CHUD_Gage.h"
#include "CHUD_Num.h"
#include "CLand3.h"
#include "CRainBow_Cloud.h"
#include "CUI_Rainbow.h"
#include "CUI_BoosterBar.h"
#include "CUI_BoosterBG.h"
#include "CMissile.h"
#include "CMissileBody.h"
#include "CMissileTarget.h"
#include "CThunder.h"
#include "CThunderCloud.h"
#include "CUI_Button.h"
#include "CUI_Timer.h"
#include "CUI_Laps.h"
#include "CUI_ItemSlot.h"
#include "CUI_ItemIcon.h"
#include "CUI_Minimap.h"
#include  "CScene3_StartBtn.h"
#include "CRenderer.h"
#include "CMinimapGround.h"
#include "CMinimapCart.h"
#include "CMagnetBody.h"
#include "CItemBox.h"
#include "CSmokeEffect.h"
#include "CWaterBombBody.h"
#include "CWaterBomb.h"
#include "CWaterBombThrow.h"
#include "CCollisionStarEffect.h"
#include "CDriftSpark.h"
#include "CWaterBombBubble.h"
#include "CDynamicCamera.h"
#include "CDustLandingEffect.h"
#include "CSpeedLine.h"
#include "SoundMgr.h"
#include "CUI_StartCountDown.h"
#include "CUI_EndCountDown.h"
#include "CBubbleEscape.h"
#include "CUI_PauseMenu.h"
#include "CPause_MenuBtn.h"
#include "CPause_ReplayBtn.h"	
#include "CPlayTimeMgr.h"


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

	if (FAILED(Ready_RenderTarget()))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Collision_Matrix()))
		return E_FAIL;

	SoundMgr::GetInstance().StopAll();
	CPlayTimeMgr::GetInstance()->SetRaceStart();
	return S_OK;
}

void CCollisionTest::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	// 충돌 처리
	//CCollider* pCartCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Cart", L"Com_Collider"));
	////CCollider* pCartSphereCollider = static_cast<CCollider*>
	////	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Cart", L"Com_SphereCollider"));
	//
	//CCollider* pBoxCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_CollisionBox", L"Com_Collider"));


	//CCollider* pRainBowCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Rainbow_Cloud", L"Com_Collider"));

	//CCollider* pBananaCollider = static_cast<CCollider*>
	//	(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic", L"Obj_Banana", L"Com_Collider"));

	//CCollisionMgr::GetInstance()->Collision(pBoxCollider, pCartCollider);
	//CCollisionMgr::GetInstance()->Collision(pRainBowCollider, pCartCollider);
	//CCollisionMgr::GetInstance()->Collision(pBananaCollider, pCartCollider);

	auto map = Get_GameObjects(L"GameLogic");
	//auto map2 = Get_GameObjects(L"Environment");

	vector<CGameObject*> objects;
	for (auto& p : map)
	{
		for (auto& pObj : p.second)
			if (!pObj->Get_Components<CCollider>().empty())
				objects.insert(objects.end(), p.second.begin(), p.second.end());
	}
	/*for (auto& p : map2)
		objects.insert(objects.end(), p.second.begin(), p.second.end());*/

	Process_Collision(objects);
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

void CCollisionTest::OnLostDevice()
{
	CScene::OnLostDevice();
	CRenderer::GetInstance()->OnLostDevice();
}

void CCollisionTest::OnResetDevice()
{
	CScene::OnResetDevice();
	CRenderer::GetInstance()->OnResetDevice(m_pGraphicDev);
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

HRESULT CCollisionTest::Ready_RenderTarget()
{
	//CRenderer::GetInstance()->Ready_RenderTarget(m_pGraphicDev, 250, 400);

	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"Minimap", 250, 400);
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

	pGameObject->SetLayer(pGameObjectLayer);
	pCartBody->Set_Child(pGameObject);
	dynamic_cast<CWheel*>(pGameObject)->ResetPrePos();

	// ## 오른쪽 앞바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_FR);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_FR", pGameObject)))
		return E_FAIL;

	pGameObject->SetLayer(pGameObjectLayer);
	pCartBody->Set_Child(pGameObject);
	dynamic_cast<CWheel*>(pGameObject)->ResetPrePos();
	
	// ## 왼쪽 뒷바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BL);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BL", pGameObject)))
		return E_FAIL;

	pGameObject->SetLayer(pGameObjectLayer);
	pCartBody->Set_Child(pGameObject);
	dynamic_cast<CWheel*>(pGameObject)->ResetPrePos();
	
	// ## 오른쪽 뒷바퀴
	pGameObject = CWheel::Create(m_pGraphicDev, WHEEL_BR);
	
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Wheel_BR", pGameObject)))
		return E_FAIL;
	pGameObject->SetLayer(pGameObjectLayer);
	pCartBody->Set_Child(pGameObject);
	dynamic_cast<CWheel*>(pGameObject)->ResetPrePos();

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
	pCartBody->Set_Child(pGameObject);


	// 연기 이펙트
	pGameObject = CSmokeEffect::Create(m_pGraphicDev);
	
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"SmokeEffect", pGameObject)))
		return E_FAIL;
	dynamic_cast<CSmokeEffect*>(pGameObject)->SetCart(pCart);

	// 충돌시 나오는 별 이펙트
	pGameObject = CCollisionStarEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"CollisionStarEffect", pGameObject)))
		return E_FAIL;
	pCartBody->Set_Child(pGameObject);

	// 착지시 먼지 이펙트
	pGameObject = CDustLandingEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"DustLandingEffect", pGameObject)))
		return E_FAIL;

	// DriftSpark
	pGameObject = CDriftSpark::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"DriftSpark", pGameObject)))
		return E_FAIL;
	pCart->Set_Child(pGameObject);

	// SpeedLine
	pGameObject = CSpeedLine::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pGameObjectLayer->Add_GameObject(L"SpeedLine", pGameObject)))
		return E_FAIL;
	static_cast<CSpeedLine*>(pGameObject)->SetCart(pCart);


	// ItemBox
	for (int i = 0; i < 5; ++i)
	{
		pGameObject = CItemBox::Create(m_pGraphicDev);
		pGameObject->Get_Transform()->Set_Pos({ -150 + i * 15.f,0,-200 });
		if (nullptr == pGameObject)
			return E_FAIL;
	
		TCHAR szBuff[32];
		wsprintf(szBuff, L"Obj_ItemBox%d", i);
		if (FAILED(pGameObjectLayer->Add_GameObject(szBuff, pGameObject)))
			return E_FAIL;
	}
	// # 플레이어 따라다니는 3인칭 카메라
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
	///////////////////////////////////////////////////////////////////////////////////////
	// 테스트용 다이나믹 카메라
	/*_vec3 vEye = { 0.f, 30.f, -30.f };
	_vec3 vAt = { 0.f, 0.f, 100.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };

	CGameObject* pDynamicCam =
		CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);

	if (pDynamicCam == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(
		L"Obj_DynamicCamera", pDynamicCam)))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(
		CAMERA_DYNAMIC,
		static_cast<CCamera*>(pDynamicCam))))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_DYNAMIC)))
		return E_FAIL;*/
	///////////////////////////////////////////////////////////////////////////////////////
	
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

	pBox = CCollisionBox::Create(m_pGraphicDev);

	if (pBox == nullptr)
		return E_FAIL;
	pBox->Get_Transform()->Set_Pos({ -200.f, 1.f, 0.f });
	//pBox->Set_CollisionLayer(CL_LAYER1);

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_CollisionBox2", pBox)))
		return E_FAIL;



	////////////////////////////////////////////////////////////////////////////////////////
	// 미사일 타겟1
	CGameObject* pMissileTarget = CMissileTarget::Create(m_pGraphicDev);
	
	if (pMissileTarget == nullptr)
		return E_FAIL;
	
	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_MissileTarget", pMissileTarget)))
		return E_FAIL;
  
	// 미사일 타겟2
	CGameObject* pMissileTarget2 = CMissileTarget::Create(m_pGraphicDev);

	if (pMissileTarget2 == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_MissileTarget2", pMissileTarget2)))
		return E_FAIL;

	pMissileTarget2->Get_Transform()->Set_Pos({ -15.f, 0.f, 80.f });

	// 미사일 타겟3
	CGameObject* pMissileTarget3 = CMissileTarget::Create(m_pGraphicDev);

	if (pMissileTarget3 == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_MissileTarget3", pMissileTarget3)))
		return E_FAIL;

	pMissileTarget3->Get_Transform()->Set_Pos({ -30.f, 0.f, 60.f });

	// 미사일 타겟4
	CGameObject* pMissileTarget4 = CMissileTarget::Create(m_pGraphicDev);

	if (pMissileTarget4 == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_MissileTarget4", pMissileTarget4)))
		return E_FAIL;

	pMissileTarget4->Get_Transform()->Set_Pos({ -15.f, 0.f, 60.f });

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

	// UI_BoosterBG
	pUIObject = CUI_BoosterBG::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_BoosterBG", pUIObject)))
		return E_FAIL;

	// UI_BoosterBar
	pUIObject = CUI_BoosterBar::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_BoosterBar", pUIObject)))
		return E_FAIL;

	// UI_BoosterBar
	pUIObject = CUI_Button::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Button", pUIObject)))
		return E_FAIL;

	// UI_Timer
	pUIObject = CUI_Timer::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Timer", pUIObject)))
		return E_FAIL;

	//UI_Laps

	pUIObject = CUI_Laps::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Laps", pUIObject)))
		return E_FAIL;

	// UI_ItemSlot
	pUIObject = CUI_ItemSlot::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ItemSlot", pUIObject)))
		return E_FAIL;

	// UI_ItemIcon
	pUIObject = CUI_ItemIcon::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ItemIcon", pUIObject)))
		return E_FAIL;

	// CUI_Minimap
	pUIObject = CUI_Minimap::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Minimap", pUIObject)))
		return E_FAIL;

	// 미니맵 Cart
	pUIObject = CMinimapCart::Create(m_pGraphicDev);

	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"MinimapCart", pUIObject)))
		return E_FAIL;

	// 미니맵 Ground
	pUIObject = CMinimapGround::Create(m_pGraphicDev);

	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"MinimapGround", pUIObject)))
		return E_FAIL;

	// CUI_StartCountDown
	pUIObject = CUI_StartCountDown::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_StartCountDown", pUIObject)))
		return E_FAIL;

	// CUI_EndCountDown
	pUIObject = CUI_EndCountDown::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_EndCountDown", pUIObject)))
		return E_FAIL;

	// 임시 버블 공격시 키보드 UI
	CGameObject* pBubbleEscape = CBubbleEscape::Create(m_pGraphicDev);

	if (pBubbleEscape == nullptr)
		return E_FAIL;

	if (FAILED(pUILayer->Add_GameObject(L"UI_BubbleEscape", pBubbleEscape)))
		return E_FAIL;



	//PauseMenu

	CGameObject* pPauseMenu = CUI_PauseMenu::Create(m_pGraphicDev);

	if (pPauseMenu == nullptr)
		return E_FAIL;

	if (FAILED(pUILayer->Add_GameObject(L"UI_MenuPause", pPauseMenu)))
		return E_FAIL;
	pPauseMenu->SetLayer(pUILayer);
	
	


	pUIObject = CPause_ReplayBtn::Create(m_pGraphicDev);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"Pause_ReplayBtn", pUIObject)))
		return E_FAIL;
	//pPauseMenu->Set_Child(pUIObject);

	pUIObject = CPause_MenuBtn::Create(m_pGraphicDev);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"Pause_MenuBtn", pUIObject)))
		return E_FAIL;
	//pPauseMenu->Set_Child(pUIObject);


	////////////////////////////////////////////////////////////////////////////////
	//// 임시 쉴드1 UI
	//CGameObject* pShield1 = CUI_Shield1::Create(m_pGraphicDev);

	//if (pShield1 == nullptr)
	//	return E_FAIL;

	//if (FAILED(pUILayer->Add_GameObject(L"UI_Shield1", pShield1)))
	//	return E_FAIL;

	//// 임시 쉴드2 UI
	//CGameObject* pShield2 = CUI_Shield2::Create(m_pGraphicDev);

	//if (pShield2 == nullptr)
	//	return E_FAIL;

	//if (FAILED(pUILayer->Add_GameObject(L"UI_Shield2", pShield2)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CCollisionTest::Ready_Collision_Matrix()
{
	//Set_CollisionMatrix(CL_DEFAULT, CL_LAYER1, false);

	return S_OK;
}

void CCollisionTest::Free()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
	CRenderer::GetInstance()->Delete_RenderTarget(L"Minimap");
	CScene::Free();
}
