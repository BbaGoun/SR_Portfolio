#include "CProtoMgr.h"

IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
    Free();
}

HRESULT CProtoMgr::Ready_Prototype(const WCHAR* tag, CComponent* comp, ProtoKind ePK, bool addable, const WCHAR* displayName)
{
    auto iter = find_if(m_mapComponent.begin(),
        m_mapComponent.end(),
        CTag_Finder(tag));

    if (iter != m_mapComponent.end())
        return E_FAIL;

    ProtoRecord pr = {};
    wcsncpy_s(pr.tag, tag, 256);
    wcsncpy_s(pr.name, displayName, 256);
    pr.kind = ePK;
    pr.addable = addable;
    pr.proto = comp;

    m_mapComponent.insert({ tag, pr });

    return S_OK;
}

CComponent* CProtoMgr::Get_CloneComponent(const WCHAR* pComponentTag)
{
    auto        iter = find_if(m_mapComponent.begin(),
        m_mapComponent.end(),
        CTag_Finder(pComponentTag));

    if (iter == m_mapComponent.end())
        return nullptr;

    return iter->second.proto->Clone();
}

void CProtoMgr::Free()
{
    for_each(m_mapComponent.begin(), m_mapComponent.end(), [](pair<const _tchar*, ProtoRecord> p)->void {
        Safe_Release(p.second.proto);
        });
    m_mapComponent.clear();
}
