#include "CScene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_CollisionMatrix.set();
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

HRESULT CScene::Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pGameObject)
{
    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(m_mapLayer.find(pLayerTag)->second->Add_GameObject(pObjTag, pGameObject)))
        return E_FAIL;
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

void CScene::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->FixedUpdate_Layer(fFixedDeltaTime);
}

_int CScene::Update_Scene(const _float& fDeltaTime)
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->Update_Layer(fDeltaTime);

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fDeltaTime)
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->LateUpdate_Layer(fDeltaTime);
}

void CScene::Render_Scene()
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->Render_Layer();
}

void CScene::Set_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer, bool bCollision)
{
    if (srcLayer > dstLayer)
        swap(srcLayer, dstLayer);

    int index = (31 - srcLayer + 1) * (31 - srcLayer) * 0.5f + (31 - dstLayer);
    m_CollisionMatrix.set(index, bCollision);
}

void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pGraphicDev);
}
