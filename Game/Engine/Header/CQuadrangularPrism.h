#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CQuadrangularPrism : public CVIBuffer
{
protected:
    explicit CQuadrangularPrism();
    explicit CQuadrangularPrism(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CQuadrangularPrism(const CQuadrangularPrism& rhs);
    virtual ~CQuadrangularPrism() override;

public:
    HRESULT Ready_Buffer() override;
    void    SetColor(D3DXCOLOR Color) { m_Color = Color; }

    static CQuadrangularPrism* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

private:
    D3DXCOLOR   m_Color;

protected:
    virtual void		Free() override;
};

END
