#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain :
    public CVIBuffer
{
protected:
    explicit CTerrain();
    explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrain(const CTerrain& rhs);
    virtual ~CTerrain() override;

public:
    const vector<_vec3>& GetPoints() {return m_vecPoints; }

public:
    HRESULT Ready_Buffer() override;

    static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

private:
    vector<_vec3> m_vecPoints;

protected:
    virtual void		Free() override;
};

END