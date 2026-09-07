#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCartBodyCol : public CVIBuffer
{
protected:
    explicit CCartBodyCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCartBodyCol(const CCartBodyCol& rhs);
    virtual ~CCartBodyCol() override;

public:
    HRESULT Ready_Buffer() override;

    static CCartBodyCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
