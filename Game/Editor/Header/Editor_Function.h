#pragma once
#include "CGameObject.h"

inline CGameObject* FindByGuid(uint64_t guid, const map<const _tchar*, CGameObject*>& objMap)
{
    for (auto& p : objMap) {
        if (p.second == nullptr)
            continue;

        if (guid == p.second->GetGuid())
            return p.second;
    }
    return nullptr;
}

