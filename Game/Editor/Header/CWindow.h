#pragma once
#include "CBase.h"
#include "pch.h"

class CWindow :
    public CBase
{
protected:
    explicit CWindow();
    explicit CWindow(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CWindow(const CWindow& rhs);
    virtual ~CWindow();
    
public:
    virtual void Update_Window() = 0;
    virtual void LateUpdate_Window() {};

protected:
    LPDIRECT3DDEVICE9			m_pGraphicDev;
    _bool						m_bClone;

public:
    virtual HRESULT Ready_Window() = 0;
    virtual void InvalidateDeviceObjects() = 0;
    virtual CWindow* Clone() = 0;

protected:
    virtual void Free();
};

