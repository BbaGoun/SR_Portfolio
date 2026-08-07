#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCartBody : public CVIBuffer
{
protected:
    explicit CCartBody();
    explicit CCartBody(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCartBody(const CCartBody& rhs);
    virtual ~CCartBody() override;

public:
    HRESULT Ready_Buffer() override;

    static CCartBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
