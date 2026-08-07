#include "pch.h"
#include "CStage.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CFontMgr.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CTerrain.h"
#include "CCamera.h"
#include "CSkyBox.h"
#include "CCameraMgr.h"
#include "CCollisionMgr.h"
#include "CManagement.h"
#include "CLightMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
    
    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;
    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;
    if (FAILED(Ready_BLock_Layer(L"BLock_Layer")))
         return E_FAIL;
    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int    iExit = CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    CScene::LateUpdate_Scene(fTimeDelta);

    CCollider* CPlayerCollider = dynamic_cast<CCollider*>
        (CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic_Layer", L"Player", L"Com_Collider"));
    CCollider* CMonsterCollider = dynamic_cast<CCollider*>
        (CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic_Layer", L"Monster", L"Com_Collider"));

    CCollisionMgr::GetInstance()->Collision(CMonsterCollider, CPlayerCollider);
}

void CStage::Render_Scene()
{

}

HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    CGameObject* pGameObject = nullptr;

    // MainCamera
    pGameObject = CCamera::Create(m_pGraphicDev, PLAYER1);
    
    if (nullptr == pGameObject)
        return E_FAIL;
    
    if (FAILED(pLayer->Add_GameObject(L"Camera", pGameObject)))
        return E_FAIL;
    CCameraMgr::GetInstance()->AddCamera(pGameObject);

    // MapCamera

    pGameObject = CCamera::Create(m_pGraphicDev, MAP);
    
    if (nullptr == pGameObject)
        return E_FAIL;
    
    if (FAILED(pLayer->Add_GameObject(L"MapCamera", pGameObject)))
        return E_FAIL;
    CCameraMgr::GetInstance()->AddCamera(pGameObject);


    // SkyBox
    pGameObject = CSkyBox::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
        return E_FAIL;
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    CGameObject* pGameObject = nullptr;

    // Player
    pGameObject = CPlayer::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    // CollisonMgr에 콜라이더 등록
    //CCollider* pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Collider"));
    //CCollisionMgr::GetInstance()->AddCollider(OBJ_PLAYER, pCollider);

    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;

    vector<_vec3> vMonsterPos;
    _float x = 0;
    _float y = -20;
    _float z = 0;
    _float dx = 20;
    _float dy = 20;
    _float dz = -20;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            x += dx;
            y += dy;
            vMonsterPos.push_back({ x,y,z });
        }
        dx *= -1;
        for (int j = 0; j < 3; ++j)
        {
            z += dz;
            y += dy;
            vMonsterPos.push_back({ x,y,z });
        }
        dz *= -1;
    }


    for (int i = 0; i < 1; ++i)
    {
        pGameObject = CMonster::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;

        // 위치 설정
        CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = vMonsterPos[i];

        // CollisonMgr에 콜라이더 등록
        //CCube_Collider* pCube_Collider = dynamic_cast<CCube_Collider*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_CubeCollider"));
        //m_pColliderCom->SetOffset(m_pTransformCom->m_vInfo[INFO_POS]);
        //m_pColliderCom->SetSize({ 0,0,0 });
        //CCollisionMgr::GetInstance()->AddCollider(OBJ_MONSTER, pCollider);

        const _tchar* szBuff = L"Monster";
        if (FAILED(pLayer->Add_GameObject(szBuff, pGameObject)))
            return E_FAIL;
    }


    // Terrain
    pGameObject = CTerrain::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_BLock_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    
    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Light()
{
    D3DLIGHT9       tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;


    tLightInfo.Diffuse  = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient  = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

    tLightInfo.Direction = { 1.f, -1.f, 1.f };


    if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
        return E_FAIL;
    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg", 1))))
        return E_FAIL;


    return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pStage = new CStage(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        Safe_Release(pStage);
        MSG_BOX("pStage Create Failed");
        return nullptr;
    }

    return pStage;
}

void CStage::Free()
{
    CScene::Free();
}
