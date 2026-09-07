#include "pch.h"
#include "CNexonVideo.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CRcTex.h"
#include "CCollisionTest.h"
#include "CItem.h"
#include "CUI_NexonLogo.h"
#include"CDInputMgr.h"

#include "CStartMenu.h"

CNexonVideo::CNexonVideo(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), m_pLoadingThread(nullptr)
{
}

CNexonVideo::~CNexonVideo()
{
}

HRESULT CNexonVideo::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer()))
        return E_FAIL;



    return S_OK;
}

_int CNexonVideo::Update_Scene(const _float& fDeltaTime)
{
    _int    iExit = CScene::Update_Scene(fDeltaTime);
   CUI_NexonLogo* pVideo = dynamic_cast<CUI_NexonLogo*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI",L"CUI_NexonLogo"));

    if (pVideo->GetFinished()==true)
    {
        Engine::CScene* pStage = CStartMenu::Create(m_pGraphicDev);
   
        if (nullptr == pStage)
            return E_FAIL;
   
        if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
        {
            MSG_BOX("Stage Create Failed");
            return -1;
        }
    }
   

    //if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_SPACE))
    //{
    //    //Engine::CScene* pStage = CScene_Test::Create(m_pGraphicDev);
    //    //Engine::CScene* pStage = CItem::Create(m_pGraphicDev);
    //    //Engine::CScene* pStage = CCollisionTest::Create(m_pGraphicDev);
    //    Engine::CScene* pStage = CStartMenu::Create(m_pGraphicDev);
    //
    //    if (nullptr == pStage)
    //        return E_FAIL;
    //
    //    if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
    //    {
    //        MSG_BOX("Stage Create Failed");
    //        return -1;
    //    }
    //}

    return iExit;
}

void CNexonVideo::LateUpdate_Scene(const _float& fDeltaTime)
{
    CScene::LateUpdate_Scene(fDeltaTime);
}

void CNexonVideo::Render_Scene()
{
    _matrix matView, matProj;
    _vec3 vEye, vAt, vUp;
    vEye = { 0, 0, -2 };
    vAt = { 0, 0, 1 };
    vUp = { 0, 1, 0 };
    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DVIEWPORT9 vp;

    m_pGraphicDev->GetViewport(&vp);

    D3DXMatrixOrthoLH(&matProj, float(vp.Width), float(vp.Height), 1.f, 1000.f);

    m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

}

HRESULT CNexonVideo::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    //CGameObject* pGameObject = nullptr;

    //// BackGround
    //pGameObject = CBackGround::Create(m_pGraphicDev);
 

    //if (nullptr == pGameObject)
    //    return E_FAIL;

    //if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
    //    return E_FAIL;

    //D3DVIEWPORT9 vp;
    //m_pGraphicDev->GetViewport(&vp);
    //pGameObject->Get_Transform()->Set_Scale({ float(vp.Width), float(vp.Height), 1 });
    //pGameObject->Get_Transform()->Set_Pos({ 0, 10, 15 });


    return S_OK;
}

HRESULT CNexonVideo::Ready_Prototype()
{

    return S_OK;
}

HRESULT CNexonVideo::Ready_UI_Layer()
{
    CLayer* pUILayer = CLayer::Create();
    if (pUILayer == nullptr)
        return E_FAIL;
    m_mapLayer.insert({ L"UI", pUILayer });

    CGameObject* pUIObject = nullptr;


    pUIObject = CUI_NexonLogo::Create(m_pGraphicDev);
    if (nullptr == pUIObject)
        return E_FAIL;
    if (FAILED(pUILayer->Add_GameObject(L"CUI_NexonLogo", pUIObject)))
        return E_FAIL;

}

CNexonVideo* CNexonVideo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CNexonVideo* pLogo = new CNexonVideo(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("NexonVideo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CNexonVideo::Free()
{
    Safe_Release(m_pLoadingThread);

    CScene::Free();
}
