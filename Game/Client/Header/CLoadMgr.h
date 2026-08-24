#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Parsing.h"

class CLoadMgr :
    public CBase
{
    DECLARE_SINGLETON(CLoadMgr)

private:
    explicit	CLoadMgr();
    ~CLoadMgr() override;

public:
    void ReadyCreateMap();

    CGameObject* CreateByType(const _tchar* type, LPDIRECT3DDEVICE9 pGraphicDev);

    CGameObject* LoadGameObjectClient(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
        CGameObject* pParent, CScene* pScene);

private:
    map<const _tchar*, std::function<CGameObject* (LPDIRECT3DDEVICE9)>> m_createMap;

private:
    // CBase을(를) 통해 상속됨
    void Free() override;
};

