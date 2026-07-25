#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex :
    public CVIBuffer
{
protected:
    explicit CRcTex();
    explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRcTex(const CRcTex& rhs);
    virtual ~CRcTex() override;

public:
    HRESULT Ready_Buffer() override;

    static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END
