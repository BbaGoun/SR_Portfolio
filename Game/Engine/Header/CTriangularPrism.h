#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriangularPrism : public CVIBuffer
{
protected:
    explicit CTriangularPrism();
    explicit CTriangularPrism(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTriangularPrism(const CTriangularPrism& rhs);
    virtual ~CTriangularPrism() override;

public:
    HRESULT Ready_Buffer() override;
    void    SetColor(D3DXCOLOR Color) { m_Color = Color; }

    static CTriangularPrism* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

private:
    D3DXCOLOR   m_Color;
protected:
    virtual void		Free() override;
};

END
