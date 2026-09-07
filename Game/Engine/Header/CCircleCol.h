#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCircleCol :
    public CVIBuffer
{
protected:
    explicit CCircleCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCircleCol(const CCircleCol& rhs);
    virtual ~CCircleCol() override;

public:
    HRESULT Ready_Buffer() override;

    static CCircleCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
