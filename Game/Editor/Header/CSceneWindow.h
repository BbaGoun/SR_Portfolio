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

    void Object_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void Spline_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir);
    void HeightMap_Pick(CHeightMap* pHM, _vec3 worldRayOrigin, _vec3 worldRayDir);

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

protected:
    virtual void Free();
};

