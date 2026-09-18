#include "pch.h"
#include "CMirrorTest.h"
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
#include "CWheel.h"
#include "CBoostWind.h"
#include "CBoostJet.h"
#include "CFollowSmoothCam.h"
#include "CDustLandingEffect.h"
#include "CSpeedLine.h"
#include "CUI_StartCountDown.h"
#include "CUI_EndCountDown.h"
#include "CPlayTimeMgr.h"
#include "CStartCam.h"
#include "CTrackMgr.h"
#include "CTrackCam.h"
#include "CFinishCam.h"
#include "CSkyDome.h"
#include "CUI_RankBG.h"
#include "CUI_RankName.h"
#include "CDInputMgr.h"
#include "CRankMgr.h"
#include "CCartBot.h"
#include "CUI_Laps.h"
#include "CWaterBombBubble.h"
#include "CBoostAura.h"
#include "CShield1.h"
#include "CShield2.h"
#include "CMinimapCartBot.h"
#include "CCollisionStarEffect.h"
#include "CItemGainEffect.h"
#include "CUI_RankNumber.h"
#include "CUI_PauseMenu.h"
#include "CPause_MenuBtn.h"
#include "CPause_ReplayBtn.h"
#include "SoundMgr.h"

CMirrorTest::CMirrorTest(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CMirrorTest::~CMirrorTest()
{
}

HRESULT CMirrorTest::Ready_Scene()
{
	LoadSceneFromFile();

	SoundMgr::GetInstance().PlayBGM(L"BGM/village/village_03_re.ogg", 0.4f);

	return S_OK;
}

HRESULT CMirrorTest::PostReady_Scene()
{
	CScene::PostReady_Scene();
	CPlayTimeMgr::GetInstance()->SetRaceStartTest();

	Ready_GameLogic_Layer();
	Ready_Environment_Layer();
	Ready_UI_Layer();
	Ready_Collision_Matrix();

	//CPlayTimeMgr::GetInstance()->SetRaceStart();
	return S_OK;
}

void CMirrorTest::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CDInputMgr::GetInstance()->Record_FixedUpdate();
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	auto& map = Get_GameObjects(L"GameLogic");

	vector<CGameObject*> objects;
	objects.reserve(1000);
	for (auto& p : map) {
		for (auto& pObj : p.second)
			if (pObj->Get_Component<CCollider>())
				objects.push_back(pObj);
	}

	Process_Collision(objects);
}

_int CMirrorTest::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	CCameraMgr::GetInstance()->UpdateClosedRePlayCam();
	return iExit;
}

void CMirrorTest::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CMirrorTest::Render_Scene()
{
	//CScene::Render_Scene();
}

void CMirrorTest::OnLostDevice()
{
	CScene::OnLostDevice();
	CRenderer::GetInstance()->OnLostDevice();
}

void CMirrorTest::OnResetDevice()
{
	CScene::OnResetDevice();
	CRenderer::GetInstance()->OnResetDevice(m_pGraphicDev);
}

HRESULT CMirrorTest::LoadSceneFromFile()
{
	// 일단 넣어두기
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	const _tchar* path = nullptr;

	path = L"../Bin/Resource/Editor/AfterTest/Mirror.scene";

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
	fclose(fp);

	return S_OK;
}

HRESULT CMirrorTest::Ready_GameLogic_Layer()
{
	CGameObject* pGameObject = nullptr;
	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart");
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
	CGameObject* pPlayer = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Player");
	CGameObject* pPlayerHead = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_PlayerHead");

	static_cast<CCart*>(pCart)->SetPlayerHead(pPlayerHead);
	static_cast<CCart*>(pCart)->AddWheel();

	pCartBody->Set_ChildTuneDefault(pPlayer);
	pCartBody->Get_Transform()->Set_Pos({ 0, 0.5f, 0 });

	// 이펙트
		// ## 부스터 왼쪽1 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_L1);

	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostWindL1", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);

	// ## 부스터 왼쪽2 바람 이펙트
	// BoostWindL2
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_L2);

	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostWindL2", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);

	// ## 부스터 오른쪽1 바람 이펙트
	// BoostWindR1
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_R1);

	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostWindR1", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);
	// ## 부스터 오른쪽2 바람 이펙트
	pGameObject = CBoostWind::Create(m_pGraphicDev, WIND_R2);

	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostWindR2", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);

	// ## 부스터 제트 이펙트
	// BoostJet
	pGameObject = CBoostJet::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostJet", pGameObject);
	pCartBody->Set_ChildWithoutTune(pGameObject);

	// ## 부스터 제트 아우리
	// BoostAura
	pGameObject = CBoostAura::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BoostAura", pGameObject);
	pCartBody->Set_ChildWithoutTune(pGameObject);

	// Player Shield1,2
	pGameObject = CShield1::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"PlayerShield1", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);
	static_cast<CCart*>(pCart)->SetShield1(pGameObject);

	pGameObject = CShield2::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"PlayerShield2", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);
	static_cast<CCart*>(pCart)->SetShield2(pGameObject);

	pGameObject = CWaterBombBubble::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"BotBubble", pGameObject);
	static_cast<CCart*>(pCart)->SetBubble(pGameObject);

	// 파티클
		// 연기 이펙트
		// SpeedLine
	pGameObject = CSpeedLine::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"SpeedLine", pGameObject);
	static_cast<CSpeedLine*>(pGameObject)->SetCart(pCart);
	// Particle
		// SmokeParticle
	pGameObject = CSmokeEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"SmokeEffect", pGameObject);
	dynamic_cast<CSmokeEffect*>(pGameObject)->SetCart(pCart);

	// 충돌시 나오는 별 이펙트
	pGameObject = CCollisionStarEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"CollisionStarEffect", pGameObject);
	pCart->Set_ChildWithoutTune(pGameObject);

	// 착지시 먼지 이펙트
	// DustParticle
	pGameObject = CDustLandingEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"DustLandingEffect", pGameObject);
	static_cast<CDustLandingEffect*>(pGameObject)->SetOwner(pCart);
	static_cast<CCart*>(pCart)->SetDustLandingEffect(static_cast<CDustLandingEffect*>(pGameObject));

	//ItemGainEffect
	pGameObject = CItemGainEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"ItemGainEffect", pGameObject);

	//Camera
	// 자유 카메라
	_vec3 vEye, vAt, vUp, vLook;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vAt);
	pCart->Get_Transform()->Get_Info(INFO_UP, &vUp);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	vEye = vAt + (vUp * 8.5f) + (vLook * -15.f);
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp, D3DXToRadian(45.f));

	if (pGameObject == nullptr)
		return E_FAIL;

	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"Obj_DynamicCam", pGameObject);
	if (FAILED(CCameraMgr::GetInstance()->Ready_Camera(CAMERA_DYNAMIC,
		static_cast<CCamera*>(pGameObject))))
		return E_FAIL;

	if (FAILED(CCameraMgr::GetInstance()->SetMainCamera(CAMERA_DYNAMIC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMirrorTest::Ready_Environment_Layer()
{
	CLayer* pEnvironmentLayer = CLayer::Create();

	if (pEnvironmentLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"Environment", pEnvironmentLayer });

	CGameObject* pEnvObject = nullptr;

	//CSkyDome
	pEnvObject = CSkyDome::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;
	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_SkyDome", pEnvObject)))
		return E_FAIL;

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);

	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	m_pGraphicDev->SetLight(0, &tLightInfo);

	m_pGraphicDev->LightEnable(0, TRUE);

	return S_OK;
}


HRESULT CMirrorTest::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();

	if (pUILayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"UI", pUILayer });

	CGameObject* pUIObject = nullptr;

	return S_OK;
}

HRESULT CMirrorTest::Ready_Collision_Matrix()
{
	Set_CollisionMatrix(CL_ITEM_BOX, CL_ITEM_BOX, false);
	Set_CollisionMatrix(CL_ITEM_BOX, CL_CART_WHEEL, false);
	Set_CollisionMatrix(CL_ITEM_BOX, CL_ITEM, false);
	Set_CollisionMatrix(CL_CART_WHEEL, CL_CART_WHEEL, false);
	Set_CollisionMatrix(CL_CART_WHEEL, CL_ITEM, false);
	Set_CollisionMatrix(CL_CART_WHEEL, CL_CART_BODY, false);
	Set_CollisionMatrix(CL_ITEM, CL_ITEM, false);

	return S_OK;
}

CMirrorTest* CMirrorTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMirrorTest* pScene = new CMirrorTest(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CMirrorTest Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}
	return pScene;
}

void CMirrorTest::Free()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
	CTrackMgr::DestroyInstance();
	CRenderer::GetInstance()->Delete_BlurRT();
	CScene::Free();
}
