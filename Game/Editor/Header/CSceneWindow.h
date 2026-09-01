#pragma once
#include "CWindow.h"
#include "CGraphicDev.h"
#include "CCube.h"
#include "CHeightMap.h"

class CSceneWindow :
    public CWindow
{
protected:
    explicit CSceneWindow();
    explicit CSceneWindow(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSceneWindow(const CSceneWindow& rhs);
    virtual ~CSceneWindow();

public:
    virtual void Update_Window() override;
    virtual void LateUpdate_Window() {};

private:
    void Draw_Outline(CGameObject* pObj, D3DXCOLOR color);
    void Draw_Collider(CGameObject* pObj);
    void Draw_SplinePoints(CGameObject* pObj);
    void Draw_HeightMap(CGameObject* pObj);
    void Draw_Graph(CGameObject* pObj);

    void Object_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void Spline_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void HeightMap_Pick(CHeightMap* pHM, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void GraphNode_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void GraphPoint_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);

    void Manipulate_Object(CGameObject* pSel);
    void Manipulate_Spline(CGameObject* pSel);
    void Manipulate_GraphNode(CGameObject* pSel);
    void Manipulate_GraphPoint(CGameObject* pSel);

private:
    Engine::CCube* m_pCubeBuffer = nullptr;
    LPDIRECT3DTEXTURE9 m_pSceneTex;
    LPDIRECT3DSURFACE9 m_pSceneDepth;
    UINT m_rtW, m_rtH;

public:
    virtual void InvalidateDeviceObjects() override;
    virtual HRESULT Ready_Window() override;
    virtual CWindow* Clone() override;
    static CSceneWindow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _vec3 m_vEye = { -4.5f, 6.f, -5.f };
    float m_yaw = 0.74f, m_pitch = -0.74f;
    _vec3 m_vUp = { 0.f, 1.f, 0.f };
    _matrix m_matView, m_matInvView;
    _matrix m_matProj, m_matInvProj;
    float m_gizmoSize = 0.1f;
    float m_moveSpeed = 0.2f;
    bool m_bWasUsingTranslate = false;

protected:
    virtual void Free();
};

