#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPyramid :
    public CVIBuffer
{
protected:
    explicit CPyramid(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CPyramid(const CPyramid& rhs);
    virtual ~CPyramid() override;

public:
    HRESULT Ready_Buffer() override;
    void Render_Buffer() override;

    static CPyramid* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    virtual void		Free() override;
};

END