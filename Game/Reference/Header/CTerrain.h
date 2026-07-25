#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain :
    public CVIBuffer
{
protected:
    explicit CTerrain();
    explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrain(const CTerrain& rhs);
    virtual ~CTerrain() override;

public:
    HRESULT Ready_Buffer() override;

    static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END