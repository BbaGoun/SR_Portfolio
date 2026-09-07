#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CArrowTex :
    public CVIBuffer
{
protected:
    explicit CArrowTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CArrowTex(const CArrowTex& rhs);
    virtual ~CArrowTex() override;

public:
    HRESULT Ready_Buffer() override;

    static CArrowTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
