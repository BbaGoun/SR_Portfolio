#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCartWheelCol :  public CVIBuffer
{
protected:
    explicit CCartWheelCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCartWheelCol(const CCartWheelCol& rhs);
    virtual ~CCartWheelCol() override;

public:
    HRESULT Ready_Buffer() override;
    void Render_Buffer() override;

    static CCartWheelCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

private:
    int m_iSegment = 16;

protected:
    virtual void		Free() override;
};

END
