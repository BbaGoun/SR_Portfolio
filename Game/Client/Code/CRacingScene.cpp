#include "pch.h"
#include "CRacingScene.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CSmokeEffect.h"
#include "CSkyBox.h"
#include "CHUD_Main.h"
#include "CHUD_Gage.h"
#include "CHUD_Num.h"
#include "CUI_Rainbow.h"
#include "CUI_BoosterBar.h"
#include "CUI_BoosterBG.h"
#include "CUI_Button.h"
#include "CUI_Timer.h"
#include "CUI_ItemSlot.h"
#include "CUI_ItemIcon.h"
#include "CUI_Minimap.h"
#include "CMinimapGround.h"
#include "CMinimapCart.h"
#include "CDynamicCamera.h"
#include "CCameraMgr.h"
#include "Engine_Parsing.h"
#include "CLoadMgr.h"
#include "CCart.h"
#include "CCartBody.h"
#include "CCartBody1.h"
#include "CCartBody2.h"
#include "CWheel.h"
#include "CBoostWind.h"
#include "CBoostJet.h"
#include "CFollowSmoothCam.h"

CRacingScene::CRacingScene(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CRacingScene::~CRacingScene()
{
}

HRESULT CRacingScene::Ready_Scene()
{
	LoadSceneFromFile();

	Ready_RenderTarget();
	Ready_GameLogic_Layer();
	Ready_Environment_Layer();
	Ready_UI_Layer();
	Ready_Collision_Matrix();

	return S_OK;
}

void CRacingScene::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	auto map = Get_GameObjects(L"Default");

	vector<CGameObject*> objects;
	objects.reserve(1000);
	for (auto& p : map){
		for (auto& pObj : p.second)
			if (!pObj->Get_Component<CCollider>())
				objects.push_back(pObj);
	}

	Process_Collision(objects);
}

_int CRacingScene::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CRacingScene::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CRacingScene::Render_Scene()
{
	//CScene::Render_Scene();
}

void CRacingScene::OnLostDevice()
{
	CScene::OnLostDevice();
	CRenderer::GetInstance()->OnLostDevice();
}

void CRacingScene::OnResetDevice()
{
	CScene::OnResetDevice();
	CRenderer::GetInstance()->OnResetDevice(m_pGraphicDev);
}

HRESULT CRacingScene::LoadSceneFromFile()
{
	// 일단 넣어두기
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"Default", pGameObjectLayer });

	const _tchar* path = nullptr;
	switch (m_eMapId) {
	case MAP_TEST:
		path = L"../Bin/Resource/Editor/Scene/Test.scene";
		break;
	case MAP_FOREST:
		break;
	case MAP_VILLAGE:
		break;
	default:
		return E_FAIL;
	}

	FILE* fp = nullptr;
	if (_wfopen_s(&fp, path, L"r, ccs=UTF-8") != 0 || !fp)
		return E_FAIL;

	FileReadState st;
	st.fp = fp;

	wchar_t* t = nullptr;
	CGameObject* pRoot = nullptr;
	while (true) {
		if (st.Next(t) && !wcscmp(t, L"OBJECT"))
			pRoot = CLoadMgr::GetInstance()->LoadGameObjectClient(st, m_pGraphicDev, nullptr, this);
		else
			break;
	}
	int a = 2;
	fclose(fp);

	return S_OK;
}

HRESULT CRacingScene::Ready_RenderTarget()
{
	//CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"Minimap", 250, 400);
	return S_OK;
}

HRESULT CRacingScene::Ready_GameLogic_Layer()
{
	// 게임 오브젝트
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	CGameObject* pGameObject = nullptr;

	//// # 자유 카메라
	//_vec3 vEye = { 0, 0, -5 }, vAt = { 0, 0, 0 };
	//_vec3 vUp = { 0, 1, 0 };

	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);

	//if (pGameObject == nullptr)
	//	return E_FAIL;

	//if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_DynamicCam", pGameObject)))
	//	return E_FAIL;

	//if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_DYNAMIC,
	//	static_cast<CCamera*>(pGameObject))))
	//	return E_FAIL;

	//if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_DYNAMIC)))
	//	return E_FAIL;


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

	pGameObject = nullptr;
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

	return S_OK;
}

HRESULT CRacingScene::Ready_Environment_Layer()
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

	return S_OK;
}


HRESULT CRacingScene::Ready_UI_Layer()
{
	//CLayer* pUILayer = CLayer::Create();

	//if (pUILayer == nullptr)
	//	return E_FAIL;

	//m_mapLayer.insert({ L"UI", pUILayer });

	//CGameObject* pUIObject = nullptr;

	//// UI_HUDMain
	//pUIObject = CHUD_Main::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_HUDMain", pUIObject)))
	//	return E_FAIL;

	//// UI_HUDGage
	//pUIObject = CHUD_Gage::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_HUDGage", pUIObject)))
	//	return E_FAIL;

	//// UI_HUDNum
	//pUIObject = CHUD_Num::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_HUDNum", pUIObject)))
	//	return E_FAIL;

	//// UI_Rainbow
	//pUIObject = CUI_Rainbow::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_Rainbow", pUIObject)))
	//	return E_FAIL;

	//// UI_BoosterBG
	//pUIObject = CUI_BoosterBG::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_BoosterBG", pUIObject)))
	//	return E_FAIL;

	//// UI_BoosterBar
	//pUIObject = CUI_BoosterBar::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_BoosterBar", pUIObject)))
	//	return E_FAIL;

	//// UI_BoosterBar
	//pUIObject = CUI_Button::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_Button", pUIObject)))
	//	return E_FAIL;

	//// UI_Timer
	//pUIObject = CUI_Timer::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_Timer", pUIObject)))
	//	return E_FAIL;

	//// UI_ItemSlot
	//pUIObject = CUI_ItemSlot::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_ItemSlot", pUIObject)))
	//	return E_FAIL;

	//// UI_ItemIcon
	//pUIObject = CUI_ItemIcon::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"UI_ItemIcon", pUIObject)))
	//	return E_FAIL;

	//// CUI_Minimap
	//pUIObject = CUI_Minimap::Create(m_pGraphicDev);
	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"PreviewCart", pUIObject)))
	//	return E_FAIL;


	//// 미니맵 Cart
	//pUIObject = CMinimapCart::Create(m_pGraphicDev);

	//if (nullptr == pUIObject)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"MinimapCart", pUIObject)))
	//	return E_FAIL;

	//pUIObject = CMinimapGround::Create(m_pGraphicDev);

	//if (pUIObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"Env_MinimapGround", pUIObject)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CRacingScene::Ready_Collision_Matrix()
{
	Set_CollisionMatrix(CL_DEFAULT, CL_LAYER1, false);

	return S_OK;
}

CRacingScene* CRacingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev, MAP_ID eID)
{
	CRacingScene* pScene = new CRacingScene(pGraphicDev);
	pScene->m_eMapId = eID;

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CRacingScene Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

void CRacingScene::Free()
{
	//CRenderer::GetInstance()->Delete_RenderTarget(L"Minimap");
	CScene::Free();
}
