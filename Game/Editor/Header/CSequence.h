#pragma once
#include "CWindow.h"
#include "CGraphicDev.h"
#include "CCube.h"

class CSequence :
    public CWindow
{
protected:
    explicit CSequence();
    explicit CSequence(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSequence(const CSequence& rhs);
    virtual ~CSequence();

public:
    virtual void Update_Window() override;
    virtual void LateUpdate_Window() {};

private:
    Engine::CCube* m_pCubeBuffer = nullptr;
    LPDIRECT3DTEXTURE9 m_pSceneTex;
    LPDIRECT3DSURFACE9 m_pSceneDepth;
    UINT m_rtW, m_rtH;

public:
    virtual void InvalidateDeviceObjects() override;
    virtual HRESULT Ready_Window() override;
    virtual CWindow* Clone() override;
    static CSequence* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void Free();
};

