#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCartWheel :  public CVIBuffer
{
protected:
    explicit CCartWheel();
    explicit CCartWheel(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCartWheel(const CCartWheel& rhs);
    virtual ~CCartWheel() override;

public:
    HRESULT Ready_Buffer() override;

    static CCartWheel* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
