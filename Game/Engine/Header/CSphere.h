#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphere :
    public CVIBuffer
{
protected:
    explicit CSphere(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSphere(const CSphere& rhs);
    virtual ~CSphere() override;

public:
    HRESULT Ready_Buffer() override;

public:
    vector<VTXTEX>& GetVertices() { return m_vecVertices; }
    vector<FACE32>& GetFaces() { return m_vecFaces; }


public:
    static CSphere* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    vector<VTXTEX>			m_vecVertices;
    vector<FACE32>			m_vecFaces;

protected:
    virtual void		Free() override;
};

END