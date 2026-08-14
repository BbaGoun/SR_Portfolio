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

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Cody", Engine::CCody::Create(m_pGraphicDev))))
        return E_FAIL;

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

    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Terrain", Engine::CTerrain::Create(m_pGraphicDev))))
    //    return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Terrain2", Engine::CTerrain2::Create(m_pGraphicDev))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Terrain3", Engine::CTerrain3::Create(m_pGraphicDev))))
        return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Texture Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2))))
    //    return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger3.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Cart/WoodBoxdds.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_V_ItemBoxTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Cart/V_ItemBoxdds.dds", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_EffectTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterWindL", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterWind/BoosterWindL%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterWindR", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterWind/BoosterWindR%d.png", 2))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterJet", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Effect/BoosterJet/blue_tail.png", 1))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDMain", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/tachoBG_2.png", 1))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDGage", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/gage%d.png", 5))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HUDNum", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/VehicleHUD/Numbers/number%d.png", 10))))
        return E_FAIL;

    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BoosterIcon", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/Item_Boost.png", 1))))
    //    return E_FAIL;

    EnterCriticalSection(Get_Crt());
    lstrcpy(m_szLoading, L"Etc Loading.....................................");
    LeaveCriticalSection(Get_Crt());

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CubeCollider", Engine::CCube_Collider::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SphereCollider", Engine::CSphere_Collider::Create(m_pGraphicDev))))
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
