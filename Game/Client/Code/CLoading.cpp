#include "pch.h"
#include "CLoading.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CFontMgr.h"
#include "CScene_Test.h"
#include "CManagement.h"
#include "CRcTex.h"
#include "CCollisionTest.h"
#include "CItem.h"
#include "CStartMenu.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), m_pLoadingThread(nullptr)
{
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;    
    
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    m_pLoadingThread = CLoadingThread::Create(m_pGraphicDev, CLoadingThread::LOADING_STAGE);
   
    if (nullptr == m_pLoadingThread)
        return E_FAIL;

    return S_OK;
}

_int CLoading::Update_Scene(const _float& fDeltaTime)
{
    _int    iExit = CScene::Update_Scene(fDeltaTime);

    if (m_pLoadingThread->Get_Finish())
    {
        if (GetAsyncKeyState(VK_RETURN))
        {
            //Engine::CScene* pStage = CScene_Test::Create(m_pGraphicDev);
            //Engine::CScene* pStage = CItem::Create(m_pGraphicDev);
            //Engine::CScene* pStage = CCollisionTest::Create(m_pGraphicDev);
            Engine::CScene* pStage = CStartMenu::Create(m_pGraphicDev);

            if (nullptr == pStage)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
            {
                MSG_BOX("Stage Create Failed");
                return -1;
            }
        }
    }

    return iExit;
}

void CLoading::LateUpdate_Scene(const _float& fDeltaTime)
{
    CScene::LateUpdate_Scene(fDeltaTime);
}

void CLoading::Render_Scene()
{
    _matrix matView, matProj;
    _vec3 vEye, vAt, vUp;
    vEye = { 0, 0, -1 };
    vAt = { 0, 0, 1 };
    vUp = { 0, 1, 0 };
    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DVIEWPORT9 vp;

    m_pGraphicDev->GetViewport(&vp);
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), float(vp.Width) / vp.Height,
        1.f, 1000.f);

    m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

    CScene::Render_Scene();

    _vec2       vPos = { 100.f, 100.f };

    _tchar bufStr[128];

    CFontMgr::GetInstance()->Render_Font(L"Font_Jinji", m_pLoadingThread->Get_String(bufStr), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLoading::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;
    
    m_mapLayer.insert({ pLayerTag, pLayer });

    CGameObject* pGameObject = nullptr;

    // BackGround
    pGameObject = CBackGround::Create(m_pGraphicDev);
    
    if (nullptr == pGameObject)
        return E_FAIL;
    
    if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
        return E_FAIL;    
 
    pGameObject->Get_Transform()->Set_Pos({ 0, 0, 1 });
    pGameObject->Get_Transform()->Set_Scale({ 2 * 16.f/9.f, 2, 1 });

    return S_OK;
}

HRESULT CLoading::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;
    
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;


    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/dog0.jpg", 1))))
    //    return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Cart/UI/BackGround/scene%d.png", 2))))
        return E_FAIL;
                                                                                                                              
    return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLoading* pLogo = new CLoading(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CLoading::Free()
{
    Safe_Release(m_pLoadingThread);

    CScene::Free();
}
