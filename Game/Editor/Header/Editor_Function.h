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

inline std::wstring GetSceneName(const wchar_t* path)
{
    const wchar_t* slash = wcsrchr(path, L'\\');
    const wchar_t* slash2 = wcsrchr(path, L'/');
    if (slash2 && (!slash || slash2 > slash))
        slash = slash2;

    const wchar_t* file = slash ? slash + 1 : path;

    const wchar_t* dot = wcsrchr(file, L'.');
    if (dot && dot != file)
        return std::wstring(file, dot); // Forest
    return file;                         // 확장자 없음
}
