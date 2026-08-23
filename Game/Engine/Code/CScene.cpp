#include "CScene.h"
#include "CCollisionMgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    for (auto& u : m_CollisionMatrix)
        u |= ~0u;
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CGameObject* CScene::Find_GameObjectByTag(const _tchar* pLayerTag, const _tchar* pObjTag)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Find_GameObjectByTag(pObjTag);
}

const vector<CGameObject*>& CScene::Find_GameObjectsByTag(const _tchar* pLayerTag, const _tchar* pObjTag)
{
    static vector<CGameObject*> s_empty;

    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

    if (iter == m_mapLayer.end())
        return s_empty;

    return iter->second->Find_GameObjectsByTag(pObjTag);
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

const map<const _tchar*, vector<CGameObject*>>& CScene::Get_GameObjects(const _tchar* pLayerTag)
{
    static map<const _tchar*, vector<CGameObject*>> s_empty;
    auto it = m_mapLayer.find(pLayerTag);
    if (it == m_mapLayer.end() || !it->second)
        return s_empty;
    return it->second->Get_GameObjects();
}

HRESULT CScene::Delete_GameObject(const _tchar* pLayerTag, CGameObject* _pObj, bool bEditor)
{
    m_mapLayer.find(pLayerTag)->second->Delete_GameObject(_pObj, bEditor);

    return S_OK;
}

const vector<CGameObject*>& CScene::Get_Roots(const _tchar* pLayerTag)
{
    static const vector<CGameObject*> s_empty;
    auto it = m_mapLayer.find(pLayerTag);
    if (it == m_mapLayer.end() || !it->second)
        return s_empty;
    return it->second->Get_Roots();
}

void CScene::Attach_Root(CGameObject* _pObj)
{
    if (CLayer* pLayer = Find_Layer_Of(_pObj))
        pLayer->Attach_Root(_pObj);
}

void CScene::Detach_Root(CGameObject* _pObj)
{
    for (auto& p : m_mapLayer)
    {
        if (p.second)
            p.second->Detach_Root(_pObj);
    }
}

void CScene::Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc)
{
    if (CLayer* pLayer = Find_Layer_Of(_pDst))
        pLayer->Insert_Root_Before(_pDst, _pSrc);
}

void CScene::Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc)
{
    if (CLayer* pLayer = Find_Layer_Of(_pDst))
        pLayer->Insert_Root_After(_pDst, _pSrc);
}

CLayer* CScene::Find_Layer_Of(CGameObject* pObj)
{
    if (!pObj)
        return nullptr;
    for (auto& p : m_mapLayer)
    {
        if (p.second && p.second->Contains(pObj))
            return p.second;
    }
    return nullptr;
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

void CScene::Process_Collision(const vector<CGameObject*>& vecObjects)
{
    // GameLogic에서 오브젝트 A, B 꺼내기
    for (auto it1 = vecObjects.begin(); it1 != vecObjects.end(); ++it1) {
        CGameObject* pObj1 = *it1;
        for (auto it2 = it1 + 1; it2 != vecObjects.end(); ++it2) {
            CGameObject* pObj2 = *it2;

            // nullptr 무시
            if (!pObj1 || !pObj2)
                continue;
            // 같은 오브젝트 무시
            if (pObj1 == pObj2)
                continue;
            // 충돌 레이어 검사
            COLLISION_LAYER CL1, CL2;
            CL1 = pObj1->Get_CollisionLayer();
            CL2 = pObj2->Get_CollisionLayer();
            if (Get_CollisionMatrix(CL1, CL2)) {
                // 모든 종류의 Collider 검사
                vector<CCollider*> colliders1 = pObj1->Get_Components<CCollider>();
                vector<CCollider*> colliders2 = pObj2->Get_Components<CCollider>();

                for (auto& col1 : colliders1) {
                    for (auto& col2 : colliders2) {
                        CCollisionMgr::GetInstance()->Collision(col1, col2);
                    }
                }
            }
        }
    }
}

void CScene::Set_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer, bool bCollision)
{
    if (bCollision) {
        m_CollisionMatrix[srcLayer] |= (1u << dstLayer);
        m_CollisionMatrix[dstLayer] |= (1u << srcLayer);
    }
    else {
        m_CollisionMatrix[srcLayer] &= ~(1u << dstLayer);
        m_CollisionMatrix[dstLayer] &= ~(1u << srcLayer);
    }
}

bool CScene::Get_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer)
{
    return (m_CollisionMatrix[srcLayer] & (1u << dstLayer)) != 0;
}

void CScene::OnLostDevice()
{
    for(auto& pLayer: m_mapLayer)
        pLayer.second->OnLostDevice();
}

void CScene::OnResetDevice()
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->OnResetDevice();
}
void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pGraphicDev);
}
