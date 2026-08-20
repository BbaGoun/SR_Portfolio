#pragma once
#include "CWindow.h"
#include "CGraphicDev.h"
#include "CCube.h"
#include "Engine_Parsing.h"

class CHierarchy :
    public CWindow
{
protected:
    explicit CHierarchy();
    explicit CHierarchy(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CHierarchy(const CHierarchy& rhs);
    virtual ~CHierarchy();

public:
    virtual void Update_Window() override;
    virtual void LateUpdate_Window() {};

private:
    void Show_Hierarchy();
    void Draw_TreeNode(CGameObject* pObj);
    void RightClick_PopUp();

    void OnLoad();
    bool OpenLoadPrefabDialog(_tchar* outPath, DWORD outChars);
    void CreatePrefabFromFile(const wchar_t* path);
    CGameObject* LoadPrefabObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev, 
        CGameObject* pParent, bool bRoot, const wchar_t* prefabPath);

    void OnSave(CGameObject* pObj, bool bSaveAs);
    bool OpenSavePrefabDialog(_tchar* outPath, DWORD outChars);
    void SavePrefabFile(CGameObject* pObj, const wchar_t* path);
    void SaveGameObject(FILE* pf, CGameObject* pObj, int depth, bool bRoot);
    void BelongPrefab(CGameObject* _pObj);

private:
    bool     m_bRenaming = false;
    bool     m_bRenameFocus = false;
    uint64_t m_uRenameGuid = 0;
    char     m_renameBuf[128] = {};
    uint64_t m_uOpenGuid = 0;

public:
    virtual void InvalidateDeviceObjects() override;
    virtual HRESULT Ready_Window() override;
    virtual CWindow* Clone() override;
    static CHierarchy* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void Free();
};

