#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CManagement.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
    , m_pParent(nullptr), m_pTransformCom(nullptr), m_iCullDistance(210'000'000)
    , m_fSpeed(0), m_vForce({ 0, 0, 0 }), m_vRotation({ 0, 0, 0 })
{
    m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
    : m_pGraphicDev(rhs.m_pGraphicDev)
    , m_pParent(rhs.m_pParent)
    , m_vecChildren(rhs.m_vecChildren)
    , m_iCullDistance(rhs.m_iCullDistance)
    , m_fSpeed(rhs.m_fSpeed), m_vForce(rhs.m_vForce), m_vRotation(rhs.m_vRotation)
{
    m_pGraphicDev->AddRef();
    Ready_GameObject();
}

CGameObject::~CGameObject()
{
}

CComponent* CGameObject::Get_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
    CComponent* pComponent = Find_Component(eID, pComponentTag);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent;
}

void CGameObject::Set_Child(CGameObject* _pGO)
{
    Insert_Child(_pGO, -1);
}

void CGameObject::Insert_Child(CGameObject* _pGO, int _iIndex)
{
    if (_pGO == nullptr)
        return;

    if (this == _pGO)
        return;

    CGameObject* ancestor = m_pParent;
    while (ancestor != nullptr) {
        if (_pGO == ancestor)
            return;
        ancestor = ancestor->m_pParent;
    }

    const bool bSameParent = (_pGO->m_pParent == this);

    _vec3 vPos;
    if (!bSameParent)
        _pGO->Get_Transform()->Get_Info(INFO_POS, &vPos);

    int iOld = -1;
    if (bSameParent)
    {
        auto it = std::find(m_vecChildren.begin(), m_vecChildren.end(), _pGO);
        if (it != m_vecChildren.end())
            iOld = (int)(it - m_vecChildren.begin());
    }

    if (_pGO->m_pParent != nullptr)
    {
        auto& vecOld = _pGO->m_pParent->m_vecChildren;
        vecOld.erase(std::remove(vecOld.begin(), vecOld.end(), _pGO), vecOld.end());
        _pGO->m_pParent = nullptr;
    }
    else
        CManagement::GetInstance()->Detach_Root(_pGO);

    if (iOld >= 0 && _iIndex > iOld)
        --_iIndex;

    if (_iIndex < 0 || _iIndex > (int)m_vecChildren.size())
        _iIndex = (int)m_vecChildren.size();

    m_vecChildren.insert(m_vecChildren.begin() + _iIndex, _pGO);
    _pGO->m_pParent = this;

    if (!bSameParent)
    {
        _matrix* pMatParent = Get_Transform()->Get_World();
        _matrix matInvParent;
        D3DXMatrixInverse(&matInvParent, 0, pMatParent);
        D3DXVec3TransformCoord(&vPos, &vPos, &matInvParent);
        _pGO->Get_Transform()->Set_Pos(vPos);
    }
}

void CGameObject::Insert_Before(CGameObject* _pGO)
{
    if (_pGO == nullptr || _pGO == this)
        return;

    if (m_pParent)
    {
        auto& vec = m_pParent->m_vecChildren;
        auto it = std::find(vec.begin(), vec.end(), this);
        int idx = (it == vec.end()) ? (int)vec.size() : (int)(it - vec.begin());
        m_pParent->Insert_Child(_pGO, idx);
    }
    else
        CManagement::GetInstance()->Insert_Root_Before(this, _pGO);
}

void CGameObject::Insert_After(CGameObject* _pGO)
{
    if (_pGO == nullptr || _pGO == this)
        return;

    if (m_pParent)
    {
        auto& vec = m_pParent->m_vecChildren;
        auto it = std::find(vec.begin(), vec.end(), this);
        int idx = (it == vec.end()) ? (int)vec.size() : (int)(it - vec.begin()) + 1;
        m_pParent->Insert_Child(_pGO, idx);
    }
    else
        CManagement::GetInstance()->Insert_Root_After(this, _pGO);
}

void CGameObject::Delete_Child(CGameObject* _pObj)
{
    if (_pObj == nullptr)
        return;

    for (auto it = m_vecChildren.begin(); it != m_vecChildren.end(); ++it) {
        if (*it == _pObj)
        {
            m_vecChildren.erase(it);
            break;
        }
    }
}

void CGameObject::To_Root()
{
    _vec3 vPos;
    Get_Transform()->Get_Info(INFO_POS, &vPos);

    if (m_pParent != nullptr)
    {
        m_pParent->Delete_Child(this);
        m_pParent = nullptr;
        Get_Transform()->Set_Pos(vPos);
    }

    CManagement::GetInstance()->Attach_Root(this);
}

HRESULT CGameObject::Ready_GameObject()
{
    m_pTransformCom = static_cast<CTransform*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Transform"));
    m_pTransformCom->Set_Owner(this);
    m_mapComponent[ID_STATIC].insert({ L"Com_Transform", m_pTransformCom });

    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fDeltaTime)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fDeltaTime);

    return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component(fDeltaTime);
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::Set_CollisionLayer(COLLISION_LAYER eID)
{
    m_iCollisionLayer = 1;
    m_iCollisionLayer << eID;
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
    auto        iter = find_if(m_mapComponent[eID].begin(),
                                m_mapComponent[eID].end(), 
                                 CTag_Finder(pComponentTag));

    if (iter == m_mapComponent[eID].end())
        return nullptr;

    return iter->second;
}

void CGameObject::Free()
{
    for (_uint i = 0; i < ID_END; ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
        m_mapComponent[i].clear();
    }

    Safe_Release(m_pGraphicDev);
}
