#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHalfSphere :
    public CVIBuffer
{
protected:
    explicit CHalfSphere(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CHalfSphere(const CHalfSphere& rhs);
    virtual ~CHalfSphere() override;

public:
    HRESULT Ready_Buffer() override;

public:
    vector<VTXTEX>& GetVertices() { return m_vecVertices; }
    vector<FACE32>& GetFaces() { return m_vecFaces; }


public:
    static CHalfSphere* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

protected:
    vector<VTXTEX>			m_vecVertices;
    vector<FACE32>			m_vecFaces;

protected:
    virtual void		Free() override;
};

END