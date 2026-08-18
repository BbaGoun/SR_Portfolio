#pragma once
#include "CWindow.h"
#include "CGraphicDev.h"
#include "CCube.h"

class CInspector :
    public CWindow
{
protected:
    explicit CInspector();
    explicit CInspector(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CInspector(const CInspector& rhs);
    virtual ~CInspector();

public:
    virtual void Update_Window() override;
    virtual void LateUpdate_Window() {};

private:
    void    Header(CGameObject* _pObj);
    void    TransformCom(CGameObject* _pObj);
    void    MeshCom(CGameObject* _pObj);
    void    ColliderComs(CGameObject* _pObj);
    void    TextureCom(CGameObject* _pObj);

    void    Add_Component_Button(CGameObject* pObj);

private:

public:
    virtual void InvalidateDeviceObjects() override;
    virtual HRESULT Ready_Window() override;
    virtual CWindow* Clone() override;
    static CInspector* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void Free();
};

