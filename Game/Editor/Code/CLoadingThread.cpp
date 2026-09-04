#include "pch.h"
#include "CLoadingThread.h"
#include "CProtoMgr.h"

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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_InnerBox", Engine::CInnerBox::Create(m_pGraphicDev)
        , true, L"InnerBox")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartBodyCol", Engine::CCartBodyCol::Create(m_pGraphicDev)
        , true, L"CartBodyCol")))
        return E_FAIL;
 
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartWheelCol", Engine::CCartWheelCol::Create(m_pGraphicDev)
        ,true ,L"CartWheelCol")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev)
        ,true, L"RcTex")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_QuadrangularPrism", Engine::CQuadrangularPrism::Create(m_pGraphicDev)
        ,true, L"QuadrangularPrism")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriangularPrism", Engine::CTriangularPrism::Create(m_pGraphicDev)
        ,true, L"TriangularPrism")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Cube", Engine::CCube::Create(m_pGraphicDev)
        ,true, L"Cube")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Sphere", Engine::CSphere::Create(m_pGraphicDev)
        ,true, L"Sphere")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HalfSphere", Engine::CHalfSphere::Create(m_pGraphicDev)
        , true, L"HalfSphere")))
        return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Texture Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Sana", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/sana.jpg", 1),
        true, L"Sana")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
        true, L"Terrain")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger3.dds", 1),
        true, L"SkyBox")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ExplosionEffect", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90),
        true, L"Explosion")))
        return E_FAIL;

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
        false, L"SplinePointNormal")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SplinePointEdit", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Editor/PointSprite/SplinePointEdit.png", 1),
        false, L"SplinePointEdit")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_F_road00", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Terrain/F_road00.png", 1),
        true, L"F_road00")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterWindL", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterWind/BoosterWindL%d.png", 2),
        true, L"BoosterWindL")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_StartFlag", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/forest_valley/Edited/StartFlag1.png", 1),
        true, L"StartFlag")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_StartFlag3", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/StartFlag3.png", 1),
        true, L"StartFlag3")))
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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_cement_12_m", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/cement_12_m.png", 1),
        true, L"cement_12_m")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RoadCorner2", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/RoadCorner2.png", 1),
        true, L"RoadCorner2")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Road1", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Map/village_hightSpeed/Edited/Road1.png", 1),
        true, L"Road1")))
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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Red", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Red.png", 1),
        true, L"Red")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Sphere_Blue", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Sphere_Blue.png", 1),
        true, L"Sphere_Blue")))
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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CartExhaustTex", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/exhaust%d.png", 2),
        true, L"CartExhaust")))
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


    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"WaterBomb", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/WaterBomb/WaterBomb.png", 1),
        true, L"WaterBomb")))
        return E_FAIL;


    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"ThrowWaterBomb", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item/WaterBomb/ThrowWaterBomb.png", 1),
        true, L"ThrowWaterBomb")))
        return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Etc Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)
        ,false, L"Transform")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeCollider", Engine::CCube_Collider::Create(m_pGraphicDev)
        ,true, L"Cube Collider")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SphereCollider", Engine::CSphere_Collider::Create(m_pGraphicDev)
        ,true, L"Sphere Collider")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Spline", Engine::CSpline::Create(m_pGraphicDev)
        , true, L"Spline")))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HeightMap", Engine::CHeightMap::Create(m_pGraphicDev)
        , true, L"HeightMap")))
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
        if (FAILED(pLoading->Loading_Stage())) {
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
