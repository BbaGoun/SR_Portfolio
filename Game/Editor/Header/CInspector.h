#pragma once
#include "CWindow.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"

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
    void    GameObjectInfo(CGameObject* _pObj);
    void    TransformCom(CGameObject* _pObj);
    void    MeshCom(CGameObject* _pObj);
    void    SplineCom(CGameObject* _pObj);
    void    HeightMapCom(CGameObject* _pObj);
    void    ColliderComs(CGameObject* _pObj);
    void    TextureCom(CGameObject* _pObj);
    void    TrackGraphCom(CGameObject* _pObj);
    void    TrackGraph_Node(CTrackGraph* pTGraph);
    void    TrackGraph_Edge(CTrackGraph* pTGraph);
    void    TrackGraph_Point(CTrackGraph* pTGraph, TrackEdge* pTE);

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

