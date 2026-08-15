#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CInnerBox :
    public CVIBuffer
{
protected:
    explicit CInnerBox(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CInnerBox(const CInnerBox& rhs);
    virtual ~CInnerBox() override;

public:
    HRESULT Ready_Buffer() override;

    static CInnerBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END