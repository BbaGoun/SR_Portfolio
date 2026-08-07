#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube :
    public CVIBuffer
{
protected:
    explicit CCube();
    explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCube(const CCube& rhs);
    virtual ~CCube() override;

public:
    HRESULT Ready_Buffer() override;

    static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END