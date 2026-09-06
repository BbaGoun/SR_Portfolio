#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL COuterBox :
    public CVIBuffer
{
protected:
    explicit COuterBox(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit COuterBox(const COuterBox& rhs);
    virtual ~COuterBox() override;

public:
    HRESULT Ready_Buffer() override;

    static COuterBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END