#pragma once
#include "CGameObject.h"

inline CGameObject* FindByGuid(uint64_t guid, const map<const _tchar*, vector<CGameObject*>>& objMap)
{
    for (auto& p : objMap) {
        for (auto& pObj : p.second) {
            if (pObj == nullptr)
                continue;

            if (guid == pObj->GetGuid())
                return pObj;
        }
    }
    return nullptr;
}

