#include "CProtoMgr.h"

IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
    Free();
}

HRESULT CProtoMgr::Ready_Prototype(const WCHAR* tag, CComponent* comp)
{
    auto iter = find_if(m_mapComponent.begin(),
        m_mapComponent.end(),
        CTag_Finder(tag));

    if (iter != m_mapComponent.end())
        return E_FAIL;

    m_mapComponent.insert({ tag, comp });

    return S_OK;
}

CComponent* CProtoMgr::Get_CloneComponent(const WCHAR* pComponentTag)
{
    auto        iter = find_if(m_mapComponent.begin(),
        m_mapComponent.end(),
        CTag_Finder(pComponentTag));

    if (iter == m_mapComponent.end())
        return nullptr;

    return iter->second->Clone();
}

void CProtoMgr::Free()
{
    for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
    m_mapComponent.clear();
}
