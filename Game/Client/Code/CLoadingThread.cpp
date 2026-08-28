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

    //MenuScene1,2,3

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_UnderBar", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Scene/UnderBar%d.png", 2))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_XButton", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Button/Button_X%d.png", 2))))
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
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterJet", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterJet/blue_tail.png", 1))))
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
    //RainBowCloud
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RainBow_Alpha", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Rainbow_Cloud/RainBow_Alpha.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RainBow_NonAlpha", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/Rainbow_Cloud/RainBow_NonAlpha.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BananaTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/banana.tga", 1))))
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

    // ItemTimer
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TimerTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Timer/playtime_%d.png", 10))))
        return E_FAIL;

    // Laps

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UI_LapsTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/Laps/Laps_%d.png", 4))))
        return E_FAIL;

    // ItemIcon
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemIconTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/ItemIcon/item%d.png", 11))))
        return E_FAIL;


    // ItemSlot
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ItemSlotTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/ItemSlot/slot0.png", 1))))
        return E_FAIL;

    //MinimapGround
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MinimapGround", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/ThumbNail/xt_minimap.png", 1))))
        return E_FAIL;

    //MinimapCart
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MinimapCart", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/MinimapCart.png", 1))))
        return E_FAIL;

  

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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartFrontNumber", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartFrontNumber.png", 1),
        true, L"CartFrontNumber")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartBolt", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/CartBolt.png", 1),
        true, L"CartBolt")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartSide", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Color/CartSide.png", 1),
        true, L"CartSide")))
        return E_FAIL;

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
