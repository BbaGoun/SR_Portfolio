#include "pch.h"
#include "CLoadingThread.h"
#include "CProtoMgr.h"
#include "CMissileTex.h"
#include "CMagnetTex.h"

CLoadingThread::CLoadingThread(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev), m_bFinish(false), m_eLoadingID(LOADING_END)
{
    ZeroMemory(m_szLoading, sizeof(m_szLoading));
    m_pGraphicDev->AddRef();
}

CLoadingThread::~CLoadingThread()
{
}

HRESULT CLoadingThread::Ready_Loading(LOADINGID eID)
{
    InitializeCriticalSection(&m_Crt);

    m_eLoadingID = eID;

    m_hThread = (HANDLE)_beginthreadex(NULL, // 보안 속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
                                        0,  // 디폴트 스택 사이즈(1 바이트)
                                        Thread_Main, // 구동할 쓰레드 함수
                                        this,          // 3번 매개 변수 함수를 통해 가공할 데이터 주소
                                        0,             // 쓰레드 생성 및 실행을 조정하기 위한 옵션
                                        NULL);         // 쓰레드 ID


    return S_OK;
}

_uint CLoadingThread::Loading_Stage()
{
    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Buffer Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_InnerBox", Engine::CInnerBox::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartWheelCol", Engine::CCartWheelCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartBodyCol", Engine::CCartBodyCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_QuadrangularPrism", Engine::CQuadrangularPrism::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriangularPrism", Engine::CTriangularPrism::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Terrain2", Engine::CTerrain2::Create(m_pGraphicDev))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Terrain3", Engine::CTerrain3::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype( L"Proto_MissileTex",Engine::CMissileTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MagnetTex", Engine::CMagnetTex::Create(m_pGraphicDev))))
        return E_FAIL;
    
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ArrowCol", Engine::CArrowTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Cube", Engine::CCube::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HalfSphere", Engine::CHalfSphere::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CircleCol", Engine::CCircleCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Sphere", Engine::CSphere::Create(m_pGraphicDev)
        , true, L"Sphere")))
        return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Texture Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2))))
    //    return E_FAIL;


    //Menu UI
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene1_Item", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/1/Scene1_Item%d.png", 2))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene1_Speed", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/1/Scene1_Speed%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene1_Replay", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/1/Scene1_Replay%d.png", 2))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene2_ForestValley", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/2/Scene2_ForestValley%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene2_ClockTower", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/2/Scene2_ClockTower%d.png", 4))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_StartBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_Start%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_CharBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_Character%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_KartBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_Kart%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_ColorBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_Color%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_Map_ForestValley", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_Map_ForestValley%d.png", 3))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_ColorSetBG", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_ColorSetBG.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_ColorSet", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_ColorSet%d.png", 4))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Scene3_CharBG", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/3/Scene3_CharSetBG%d.png", 3))))
        return E_FAIL;

    //MenuScene1,2,3

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_UnderBar", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/UnderBar%d.png", 2))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_XButton", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Button/Button_X%d.png", 2))))
        return E_FAIL;

    //PauseMenu
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_PauseMenu", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Pause/PauseMenu.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Pause_ReplayBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Pause/Pause_ReplayBtn%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Pause_MenuBtn", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Pause/Pause_MenuBtn%d.png", 2))))
        return E_FAIL;

    //Stage UI

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger3.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Cart/WoodBoxdds.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_V_ItemBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Cart/V_ItemBoxdds.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterWindL", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterWind/BoosterWindL%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterWindR", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterWind/BoosterWindR%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterJet", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterJet/Boost%d.png", 2))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterJetEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterJet/V1boom02_S%d.png", 4))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ShieldEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Shield/shield%d.png", 6))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SecondShieldEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Shield/SecondShield%d.png", 3))))
        return E_FAIL;

    //HUD_UI
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDMain", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/tachoBG_2.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDGage", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/gage%d.png", 5))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDNum", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/Numbers/number%d.png", 10))))
        return E_FAIL;
    // BoosterUI
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterBar", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/BoosterGage/BoosterBar.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterBG", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/BoosterGage/BoosterBG.png", 1))))
        return E_FAIL;
    // RainBowCloud
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RainBow_Alpha", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Rainbow_Cloud/RainBow_Alpha.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RainBow_NonAlpha", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Rainbow_Cloud/RainBow_NonAlpha.png", 1))))
        return E_FAIL;

    // Banana
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BananaTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Banana/banana.png", 1))))
        return E_FAIL;

    //Thunder
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ThunderTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Thunder/Thunder%d.png", 5))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ThunderCloudTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/ThunderCloud/ThunderCloud%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ThunderPlayerEffectTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/ThunderPlayerEffect/ThunderPlayerEffect.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ThunderFloorEffectTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/ThunderFloorEffect/ThunderFloorEffect.png", 1))))
        return E_FAIL;

    // Button
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ButtonEX", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Button/Button_Speed%d.png", 4))))
        return E_FAIL;

    // TargetAim
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetAimTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/crosshaira.png", 1))))
        return E_FAIL;

    // Numbers
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_NumberTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/RacingInfoUI/Big/Time_%d.png", 14))))
        return E_FAIL;

    // ItemIcon
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemIconTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/ItemIcon/item%d.png", 11))))
        return E_FAIL;


    // ItemSlot
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemSlotTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/ItemSlot/slot0.png", 1))))
        return E_FAIL;

    // SlotChange
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemSlotChangeTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/ItemIcon/slotchanger_%d.png", 4))))
        return E_FAIL;

    //MinimapGround
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MinimapGround", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/ThumbNail/xt_minimap.png", 1))))
        return E_FAIL;

    //MinimapCart
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MinimapCart", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/MinimapCart.png", 1))))
        return E_FAIL;
    int a;

    // SkidMark Rect
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkidMark", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/SkidMark/SkidMark.png", 1))))
        return E_FAIL;
    
    // SkidMarkCircle
    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkidMark", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/SkidMark/giantShadow.png", 1))))
    //    return E_FAIL;

    // SmokeEffect
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Smoke", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Smoke/exhaust.png", 1))))
        return E_FAIL;

    // CollisionStarEffect
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CollisionStarEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Star/CollisionStar.png", 1))))
        return E_FAIL;

    // InvenWindow
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_InvenWindow", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Inven/InvenWindow%d.png", 4))))
        return E_FAIL;

    // DriftSpark
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DriftSpark", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/DriftSpark/DriftSpark.png", 1))))
        return E_FAIL;

    // SpeedLine
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SpeedLine", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/SpeedLine/speedline%d.png", 8))))
        return E_FAIL;

    // UI_StartCountDown
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_StartCountDown", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/StartCountDown/StartCountDown%d.png", 4))))
        return E_FAIL;
    // UI_EndCountDown
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_EndCountDown", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/EndCountDown/CountDown_%d.png", 11))))
        return E_FAIL;

    // SkyDome
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyDome", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/SkyDome/skydome%d.png", 4))))
        return E_FAIL;

    // RankRow
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RankRow", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/RankUI/RowMarker/Row%d.png", 8))))
        return E_FAIL;

    // RankName
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RankName", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/RankUI/Name/Name%d.png", 7))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WaterBombBody", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/WaterBomb/WaterBomb.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ThrowWaterBomb", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/WaterBomb/ThrowWaterBomb.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Shield1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Shield/Shield1.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WaterFlyEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/WaterFly/WaterFly%d.png", 3))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WaterBombBubble", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Bubble/bubble.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UfoEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Ufo/Ufo%d.png", 7))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_EmpBand", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/EmpBand/EmpBand.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BeamEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/Beam/Beam%d.png", 5))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemGainEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/ItemGainEffect/ItemGainEffect.png", 1))))
        return E_FAIL;

    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterIcon", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item_Boost.png", 1))))
    //    return E_FAIL;


    // 에디터에서 오는 것들

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodenFenceTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/WoodenFenceTest1.png", 1),
        true, L"WoodenFence")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodenBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/WoodenBox1.png", 1),
        true, L"WoodenBox")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HouseTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/dds/HouseTex.dds", 1),
        true, L"House")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SignalFlagTexture1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/Signal1.png", 1),
        true, L"Signal1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SignalFlagTexture2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/Signal2.png", 1),
        true, L"Signal2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SignalFlagTexture3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/Signal3.png", 1),
        true, L"Signal3")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ClockTowerTexture1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/dds/ClockTowerTest1.dds", 1),
        true, L"ClockTower1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ClockTowerTexture2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/dds/ClockTowerTest2.dds", 1),
        true, L"ClockTower2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ClockTowerTexture3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/dds/ClockTowerTest3.dds", 1),
        true, L"ClockTower3")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LampTexture1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/LampTest1.png", 1),
        true, L"Lamp1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LampTexture2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/LampTest2.png", 1),
        true, L"Lamp2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LampTexture3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/LampTest3.png", 1),
        true, L"Lamp3")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LampTexture4", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/LampTest4.png", 1),
        true, L"Lamp4")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CurveSignTexture1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_clockTower/Edited/CurveSign1.png", 1),
        true, L"CurveSign")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SplinePointNormal", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Editor/PointSprite/SplinePointNormal.png", 1),
        true, L"SplinePointNormal")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SplinePointEdit", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Editor/PointSprite/SplinePointEdit.png", 1),
        true, L"SplinePointEdit")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_F_road00", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Terrain/F_road00.png", 1),
        true, L"F_road00")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_StartFlag", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/StartFlag1.png", 1),
        true, L"StartFlag")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodenBridge", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/WoodenBridgeTest1.png", 1),
        true, L"WoodenBridge")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_F_ston02_1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/F_ston02_1.png", 1),
        true, L"F_ston02_1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_cement_12", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/cement_12.png", 1),
        true, L"cement_12")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_cement01", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/cement01.png", 1),
        true, L"cement01")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_cement_08", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/cement_08.png", 1),
        true, L"cement_08")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_cement_09", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/cement_09.png", 1),
        true, L"cement_09")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest1.png", 1),
        true, L"WindmillTest1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest2.png", 1),
        true, L"WindmillTest2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest3.png", 1),
        true, L"WindmillTest3")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest4", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest4.png", 1),
        true, L"WindmillTest4")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest5", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest5.png", 1),
        true, L"WindmillTest5")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WindmillTest6", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_ClockTower/Edited/WindmillTest6.png", 1),
        true, L"WindmillTest6")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_tilegrass_128_03", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/tilegrass_128_03.png", 1),
        true, L"tilegrass_128_03")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WaterTest1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/WaterTest1.png", 1),
        true, L"WaterTest1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_F_ cliff04", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/F_ cliff04.png", 1),
        true, L"F_ cliff04")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BrightBlue", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/BrightBlue.png", 1),
        true, L"BrightBlue")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DarkBlue", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/DarkBlue.png", 1),
        true, L"DarkBlue")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DarkGray", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/DarkGray.png", 1),
        true, L"DarkGray")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BrightGray", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/BrightGray.png", 1),
        true, L"BrightGray")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Black", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/Black.png", 1),
        true, L"Black")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartFrontNumber9", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartFrontNumber9.png", 1),
        true, L"CartFrontNumber9")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartFrontNumber8", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartFrontNumber8.png", 1),
        true, L"CartFrontNumber8")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartFrontNumber7", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartFrontNumber7.png", 1),
        true, L"CartFrontNumber7")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartBolt", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartBolt.png", 1),
        true, L"CartBolt")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartSide", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/CartSide.png", 1),
        true, L"CartSide")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Red", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Red.png", 1),
        true, L"Red")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_White", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/White.png", 1),
        true, L"White")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_bazzi_face00_0", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Character/Bazzi/Face/f00_0.png", 1),
        true, L"Bazzi_Face00_0")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_bazzi_face00_1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Character/Bazzi/Face/f00_1.png", 1),
        true, L"Bazzi_Face00_1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartWheelTex", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/wheel%d.png", 2),
        true, L"CartWheel")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_StartFlag3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/StartFlag3.png", 1),
        true, L"StartFlag3")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Road1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/Road1.png", 1),
        true, L"Road1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RoadCorner2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/RoadCorner2.png", 1),
        true, L"RoadCorner2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartExhaustTex", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/exhaust%d.png", 2),
        true, L"CartExhaust")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Sphere_Blue", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Sphere_Blue.png", 1),
        true, L"Sphere_Blue")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"SignFence1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/SignFence1.png", 1),
        true, L"SignFence1")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"SignFence2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/SignFence2.png", 1),
        true, L"SignFence2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"fence00_g", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/fence00_g.png", 1),
        true, L"fence00_g")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"fence04", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/fence04.png", 1),
        true, L"fence04")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Time", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/fence04.png", 1),
        true, L"fence04")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"F_mtree00_1_1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/F_mtree00_1_1.png", 1),
        true, L"F_mtree00_1_1")))

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Etc Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeCollider", Engine::CCube_Collider::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SphereCollider", Engine::CSphere_Collider::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Spline", Engine::CSpline::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HeightMap", Engine::CHeightMap::Create(m_pGraphicDev)
        , true, L"HeightMap")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MeshRibbon", Engine::CMeshRibbon::Create(m_pGraphicDev)
        , true, L"MeshRibbon")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TrackGraph", Engine::CTrackGraph::Create(m_pGraphicDev)
        , true, L"TrackGraph")))
        return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Loading Complete !!!!");
    m_bFinish = true;
    LeaveCriticalSection(Get_Crt());


    return 0;
}

unsigned int CLoadingThread::Thread_Main(void* pArg)
{
    CLoadingThread* pLoading = reinterpret_cast<CLoadingThread*>(pArg);

    int iFlag(0);

    switch (pLoading->Get_LoadingID())
    {
    case LOADING_STAGE:
        iFlag = pLoading->Loading_Stage();
        if (FAILED(iFlag))
        {
            MSG_BOX("Loading Fail");
        }
        break;

    case LOADING_BOSS:
        break;
    }

    //_endthreadex(0);

    return iFlag;       // 0 리턴 시, _endthreadex가 자동 호출
}

CLoadingThread* CLoadingThread::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
    CLoadingThread* pLoading = new CLoadingThread(pGraphicDev);

    if (FAILED(pLoading->Ready_Loading(eID)))
    {
        Safe_Release(pLoading);

        MSG_BOX("CLoadingThread Create Failed");
        return nullptr;
    }

    return pLoading;
}

void CLoadingThread::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);

    CloseHandle(m_hThread);

    DeleteCriticalSection(&m_Crt);

    Safe_Release(m_pGraphicDev);


}
