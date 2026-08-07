#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCartWheelCol :  public CVIBuffer
{
protected:
    explicit CCartWheelCol();
    explicit CCartWheelCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCartWheelCol(const CCartWheelCol& rhs);
    virtual ~CCartWheelCol() override;

public:
    HRESULT Ready_Buffer() override;

    static CCartWheelCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
